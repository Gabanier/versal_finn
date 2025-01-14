#ifndef CONSTANTS_HEAD_H
#define CONSTANTS_HEAD_H

#include "xparameters.h"

#ifndef NUM_SAMPLES
#define NUM_SAMPLES                 10
#define IMAGE_SIZE                  692224
#define OUTPUT_0_SIZE               4563
#define OUTPUT_1_SIZE               18252
#endif

#define SIZE_OF_TRANSFER            (NUM_SAMPLES * IMAGE_SIZE)
#define SIZE_OF_TRANSFER_OUT_0      (NUM_SAMPLES * OUTPUT_0_SIZE)
#define SIZE_OF_TRANSFER_OUT_1      (NUM_SAMPLES * OUTPUT_1_SIZE)

#define MEM_BASE_ADDR_HIGH          0x80000000
#define MEM_BASE_ADDR_LOW           0x0

////placeholder
#define URAM_0_BASE_ADDR            0x20200020000
#define URAM_1_BASE_ADDR            0x20200000000

#define MEM_BASE_ADDR               MEM_BASE_ADDR_LOW
#define TX0_BUFFER_BASE             (MEM_BASE_ADDR+0x00100000)
#define TX0_BUFFER_HIGH             (TX0_BUFFER_BASE + SIZE_OF_TRANSFER)
#define RX0_BUFFER_BASE             (TX0_BUFFER_HIGH+0x0000100)
#define RX0_BUFFER_HIGH             (RX0_BUFFER_BASE + SIZE_OF_TRANSFER_OUT_0*sizeof(u32))
#define RX1_BUFFER_BASE             (RX0_BUFFER_HIGH + 0x000100)
#define RX1_BUFFER_HIGH             (RX1_BUFFER_BASE + SIZE_OF_TRANSFER_OUT_1*sizeof(u32))

#define IDMA_0_ADDR                 XPAR_XAXIDMA_0_BASEADDR
#define ODMA_0_ADDR                 XPAR_XAXIDMA_1_BASEADDR
#define ODMA_1_ADDR                 XPAR_XAXIDMA_2_BASEADDR

#endif