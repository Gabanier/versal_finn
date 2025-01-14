#ifndef AXI_DMA_H
#define AXI_DMA_H
#include <xaxidma.h>
#include <sleep.h>
#include <xil_printf.h>

typedef struct {
    u8  *Tx_0;
    u32 *Rx_0;
    u32 *Rx_1;
} AddrBase;

typedef struct {
    XAxiDma *IDma_0;
    XAxiDma *ODma_0;
    XAxiDma *ODma_1;
} DmaBase;

typedef struct {
    int Image_Size;
    int Sample_Size;
    int Output_0_Size;
    int Output_1_Size;
    int sizeof_tx0;
    int sizeof_rx0;
    int sizeof_rx1;
} ExecParams;

int Init_dma(XAxiDma *AxiDma, UINTPTR BaseAddress);
int execute_buffers(AddrBase Addr, DmaBase BaseDma, ExecParams Param);
int flush_accel(AddrBase Addr, DmaBase BaseDma, ExecParams Param);

#endif