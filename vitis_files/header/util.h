#ifndef UTIL_HEADER_H
#define UTIL_HEADER_H

#include <xil_types.h>
#include "xil_printf.h"
#include <xstatus.h>
#include "sd_card.h"

typedef struct {
    int Output_0_Size;
    int Output_1_Size;
} UtilSize;

int Inference(char *FileName1, char *FileName2, u32 *RxBufferPtr_0, u32 *RxBufferPtr_1, int size);
int ByteLook(u8 *TxBufferPtr_0, u32 *RxBufferPtr_0, u32 *RxBufferPtr_1, UtilSize USize);
int ByteLookBackwards(u8 *TxBufferPtr_0, u32 *RxBufferPtr_0, u32 *RxBufferPtr_1, UtilSize USize);

#endif