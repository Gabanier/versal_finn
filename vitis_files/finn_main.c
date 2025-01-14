/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include "platform.h"
#include "constants.h"
#include <stdio.h>
#include "xtmrctr.h"

//DMA
XAxiDma IDma_0;
XAxiDma ODma_0;
XAxiDma ODma_1;

//SD CARD
static char InputFileName_0[32] =  "1:/olds/yolov3_samples.bin";
static char OutputFileName_0[32] = "1:/olds/first_exit_ver_2.bin";
static char OutputFileName_1[32] = "1:/olds/second_exit_ver_2.bin";
static char *SD_File;

//URAM WEIGHTS
static int UramCount = 2;
static int IS_URAM = 0;

static long UramAddr[2] ={URAM_0_BASE_ADDR,URAM_1_BASE_ADDR};
// static long UramAddr[1] ={-1};

static char UramFiles[2][32] = {"1:/rww/2137_0_MVAU_hls_4.dat","1:/rww/2137_1_MVAU_hls_5.dat"};

//FUNCTION DECLARATIONS
int load_runtime_weights(AddrBase Addr, DmaBase BaseDma, ExecParams Param);
XTmrCtr Timer;
u8 TmrCtrNumber;
int main()
{   

    XTmrCtr_Initialize(&Timer,XPAR_XTMRCTR_0_BASEADDR);
    
    u32 start;
    u32 end; 
    double CLK_FRQ = 1 / (double)XPAR_AXI_TIMER_0_CLOCK_FREQUENCY;
    int status_idma_0;
    int status_odma_0;
    int status_odma_1;
    int Res;

    u8 *TxBufferPtr_0;
	u32 *RxBufferPtr_0;
    u32 *RxBufferPtr_1;
    
    TxBufferPtr_0 = (u8 *)TX0_BUFFER_BASE;
	RxBufferPtr_0 = (u32 *)RX0_BUFFER_BASE;
    RxBufferPtr_1 = (u32 *)RX1_BUFFER_BASE;

    DmaBase    BaseDma = {&IDma_0, &ODma_0, &ODma_1};
    AddrBase   Addr    = {TxBufferPtr_0, RxBufferPtr_0, RxBufferPtr_1};
    ExecParams Param   = {IMAGE_SIZE, NUM_SAMPLES, OUTPUT_0_SIZE, OUTPUT_1_SIZE, sizeof(u8), sizeof(u32), sizeof(u32)};

    //Calculate total size
    size_t idma_0_size = SIZE_OF_TRANSFER * sizeof(u8);
    size_t odma_0_size = SIZE_OF_TRANSFER_OUT_0 * sizeof(u32);
    size_t odma_1_size = SIZE_OF_TRANSFER_OUT_1 * sizeof(u32);
    
    Res = SD_Init();
    if(Res != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    xil_printf("Mount of SD Successfull\r\n");

    SD_File = (char *)InputFileName_0;
    
    Res = ReadFile(SD_File, (UINTPTR)TxBufferPtr_0);
    
    if(Res != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    xil_printf("File read correctly\r\n");

    /* DEBUG TO SEE IF THE VALUES WERE LOADED CORRECTLY 
    buffer_print(TxBufferPtr_0, SIZE_OF_TRANSFER, 0);
    */
    
    status_idma_0 = Init_dma(&IDma_0,IDMA_0_ADDR);
    status_odma_0 = Init_dma(&ODma_0,ODMA_0_ADDR);
    status_odma_1 = Init_dma(&ODma_1,ODMA_1_ADDR);
    if((status_idma_0 != XST_SUCCESS) || (status_odma_0 != XST_SUCCESS) || (status_odma_1 != XST_SUCCESS)){
        xil_printf("Init of DMAs failed\r\n");
        return XST_FAILURE;
    }
    xil_printf("Init of DMAs Successfull\r\n");
    
    //Disable Intterupts
    XAxiDma_IntrDisable(&IDma_0, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_IntrDisable(&ODma_0, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
    XAxiDma_IntrDisable(&ODma_1, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

    //Flush the cache
    Xil_DCacheFlushRange((UINTPTR)TxBufferPtr_0, idma_0_size);
	Xil_DCacheFlushRange((UINTPTR)RxBufferPtr_0, odma_0_size);
    Xil_DCacheFlushRange((UINTPTR)RxBufferPtr_1, odma_1_size);

    if(IS_URAM){
        xil_printf("Loading runtime weights start\r\n");
        Res = load_runtime_weights(Addr, BaseDma, Param);
        if(Res != XST_SUCCESS) { return XST_FAILURE;}
        xil_printf("Finished loading runtime weights\r\n");
    }
    else{
        xil_printf("No runtime weights\r\n");
    }

    XTmrCtr_Start(&Timer, TmrCtrNumber);
    start = XTmrCtr_GetValue(&Timer, TmrCtrNumber);
    Res = execute_buffers(Addr, BaseDma, Param);
    end = XTmrCtr_GetValue(&Timer, TmrCtrNumber);
    XTmrCtr_Stop(&Timer, TmrCtrNumber);
    printf("time = %f\r\n",(double)(end-start)*CLK_FRQ);
    xil_printf("Finished Transfer\r\n");

    /* DEBUG TO SEE IF THE VALUES WERE LOADED CORRECTLY 
    UtilSize USize = {odma_0_size,odma_1_size};
    ByteLook(TxBufferPtr_0, RxBufferPtr_0, RxBufferPtr_1, USize);
    // ByteLookBackwards(TxBufferPtr_0, RxBufferPtr_0, RxBufferPtr_1, USize);
    */

    //Write RxBuffers to files
    SD_File = (char *)OutputFileName_0;
    Res = WriteFile(SD_File, odma_0_size, (UINTPTR)RxBufferPtr_0);
    if(Res != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    xil_printf("Write of Rx_0 Successfull\r\n");

    SD_File = (char *)OutputFileName_1;
    Res = WriteFile(SD_File, odma_1_size, (UINTPTR)RxBufferPtr_1);
    if(Res != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    xil_printf("Write of Rx_1 Successfull\r\n");

    Res = SD_Eject();
    if(Res != XST_SUCCESS)
    {
        return XST_FAILURE;
    }

    xil_printf("Unmount of SD Successfull\r\n");
    xil_printf("Exiting \r\n");
    return XST_SUCCESS;
}

int load_runtime_weights(AddrBase Addr, DmaBase BaseDma, ExecParams Param)
{   
    char *FileName;
    long addr;
    int Res;
    
    for(int i = 0; i < UramCount; i++){
        FileName = (char *)UramFiles[i];
        addr = UramAddr[i];
        Res = write_to_weights_from_file(FileName,addr);
        if(Res != XST_SUCCESS){
            xil_printf("Failed to write weights\r\n");
            return XST_FAILURE;
        }
    }
    xil_printf("Finished writing to weights\r\n");

    Res = flush_accel(Addr, BaseDma, Param);
    if(Res != XST_SUCCESS) {
        xil_printf("Failed to flush stale weights\r\n");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}
