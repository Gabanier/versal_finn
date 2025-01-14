#include "axi_dma.h"

int Init_dma(XAxiDma *AxiDma,UINTPTR BaseAddress)
{   
    XAxiDma_Config *CfgPtr;
    int status;
    CfgPtr = XAxiDma_LookupConfig(BaseAddress);
    if (!CfgPtr) {
        xil_printf("Null pointer m'es amie\r\n");
		return XST_FAILURE;
	}

    status = XAxiDma_CfgInitialize(AxiDma,CfgPtr);
    if (status != XST_SUCCESS) {
		xil_printf("Initialization failed\r\n");
		return XST_FAILURE;
	}

    if (XAxiDma_HasSg(AxiDma)) {
		xil_printf("Device configured as SG mode \r\n");
		return XST_FAILURE;
	}

    return XST_SUCCESS;
}


int execute_buffers(AddrBase Addr, DmaBase BaseDma, ExecParams Param)
{   
    int status_idma_0;
    int status_odma_0;
    int status_odma_1;

    size_t idma_0_size = Param.Image_Size * Param.sizeof_tx0;
    size_t odma_0_size = Param.Output_0_Size * Param.sizeof_rx0;
    size_t odma_1_size = Param.Output_1_Size * Param.sizeof_rx0;
    
    int flush = 0;
    if (Param.Sample_Size < 0) {flush = 1; Param.Sample_Size = 1;}

    for(int i = 0; i < Param.Sample_Size; i++){   
        status_idma_0 = XAxiDma_SimpleTransfer(BaseDma.IDma_0, (UINTPTR)&Addr.Tx_0[i*Param.Image_Size],
                        idma_0_size, XAXIDMA_DMA_TO_DEVICE);

        if (status_idma_0 != XST_SUCCESS) { return XST_FAILURE;}

        status_odma_0 = XAxiDma_SimpleTransfer(BaseDma.ODma_0, (UINTPTR)&Addr.Rx_0[i*Param.Output_0_Size],
                        odma_0_size, XAXIDMA_DEVICE_TO_DMA);

        if (status_odma_0 != XST_SUCCESS) { return XST_FAILURE;}

        status_odma_1 = XAxiDma_SimpleTransfer(BaseDma.ODma_1, (UINTPTR)&Addr.Rx_1[i*Param.Output_1_Size],
                        odma_1_size, XAXIDMA_DEVICE_TO_DMA);

        if (status_odma_1 != XST_SUCCESS) {return XST_FAILURE;}

        // while(1){
        //      if (!(XAxiDma_Busy(BaseDma.IDma_0, XAXIDMA_DMA_TO_DEVICE)) ){
        //         break;
        //     }
        //     usleep(1U);           
        // }

        while (1) {
            if (!(XAxiDma_Busy(BaseDma.IDma_0, XAXIDMA_DMA_TO_DEVICE)) ||
                (!(XAxiDma_Busy(BaseDma.ODma_0, XAXIDMA_DEVICE_TO_DMA)) &&
                !(XAxiDma_Busy(BaseDma.ODma_1, XAXIDMA_DEVICE_TO_DMA)))){
                break;
            }
            usleep(1U);
        }
        if(!flush){xil_printf("Sample received %d/%d\r\n",i+1,Param.Sample_Size);}
    }

    return XST_SUCCESS;
}

int flush_accel(AddrBase Addr, DmaBase BaseDma, ExecParams Param){
    ExecParams Params = Param;
    Params.Sample_Size = -1;
    return execute_buffers(Addr, BaseDma,Params);
}