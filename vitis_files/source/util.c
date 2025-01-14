#include "util.h"

int Inference(char *FileName1, char *FileName2, u32 *RxBufferPtr_0, u32 *RxBufferPtr_1, int size)
{
    int Res;
    size_t sum = 0;
    double mean;
    Res = ReadFile(FileName1, (UINTPTR)RxBufferPtr_0);
    if (Res) {return XST_FAILURE;}

    Res = ReadFile(FileName2, (UINTPTR)RxBufferPtr_1);
    if (Res) {return XST_FAILURE;}

    for(int i =0; i < size; i++){
        sum = sum + (RxBufferPtr_0[i] - RxBufferPtr_1[i]);            
    }
    mean = (double)sum / size;
    xil_printf("Mean val is %f", mean);
    return XST_SUCCESS;
}

int ByteLook(u8 *TxBufferPtr_0, u32 *RxBufferPtr_0, u32 *RxBufferPtr_1, UtilSize USize)
{
    int status_1 = XST_SUCCESS;
    for(int i = 0 ; i < USize.Output_0_Size; i++)
    {
        u8 byte0 = (RxBufferPtr_0[i] >> 0) & 0xFF;  // Extract Byte0
        u8 byte1 = (RxBufferPtr_0[i] >> 8) & 0xFF;  // Extract Byte1
        u8 byte2 = (RxBufferPtr_0[i] >> 16) & 0xFF; // Extract Byte2
        u8 byte3 = (RxBufferPtr_0[i] >> 24) & 0xFF; // Extract Byte3
        xil_printf("Tx val = %lu ||", TxBufferPtr_0[i]);
        xil_printf("Rx_1 val = %i \r\n", RxBufferPtr_0[i]); 
        xil_printf("Byte_0 = %lu \r\n", byte0);
        xil_printf("Byte_1 = %lu \r\n", byte1);  
        xil_printf("Byte_2 = %lu \r\n", byte2);  
        xil_printf("Byte_3 = %lu \r\n", byte3);
        if(TxBufferPtr_0[i] != RxBufferPtr_0[i]){
            status_1 = XST_FAILURE;
        }
    }

    xil_printf("status_1 == %i",status_1);
    for(int i = 0 ; i < 10; i++){xil_printf("++++++++++++++++++++++++++++++\r\n");}

    int status_2 = XST_SUCCESS;
    for(int i = 0; i < USize.Output_1_Size; i++){
        u8 byte0 = (RxBufferPtr_1[i] >> 0) & 0xFF;  // Extract Byte0
        u8 byte1 = (RxBufferPtr_1[i] >> 8) & 0xFF;  // Extract Byte1
        u8 byte2 = (RxBufferPtr_1[i] >> 16) & 0xFF; // Extract Byte2
        u8 byte3 = (RxBufferPtr_1[i] >> 24) & 0xFF; // Extract Byte3
        xil_printf("Tx val = %lu ||", TxBufferPtr_0[i]);
        xil_printf("Rx_2 val = %i \r\n", RxBufferPtr_1[i]); 
        xil_printf("Byte_0 = %lu \r\n", byte0);
        xil_printf("Byte_1 = %lu \r\n", byte1);  
        xil_printf("Byte_2 = %lu \r\n", byte2);  
        xil_printf("Byte_3 = %lu \r\n", byte3);     
        if(TxBufferPtr_0[i] != RxBufferPtr_1[i]){
            status_2 = XST_FAILURE;
        }
    }

    if(status_1 != XST_SUCCESS || status_2 != XST_SUCCESS){
        xil_printf("Vals not consistent \r\n");
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}

int ByteLookBackwards(u8 *TxBufferPtr_0, u32 *RxBufferPtr_0, u32 *RxBufferPtr_1, UtilSize USize)
{
    int status_1 = XST_SUCCESS;
    for(int i = USize.Output_0_Size ; i > -1 ; i--)
    {
        u8 byte0 = (RxBufferPtr_0[i] >> 0) & 0xFF;  // Extract Byte0
        u8 byte1 = (RxBufferPtr_0[i] >> 8) & 0xFF;  // Extract Byte1
        u8 byte2 = (RxBufferPtr_0[i] >> 16) & 0xFF; // Extract Byte2
        u8 byte3 = (RxBufferPtr_0[i] >> 24) & 0xFF; // Extract Byte3
        xil_printf("Tx val = %lu ||", TxBufferPtr_0[i]);
        xil_printf("Rx_1 val = %i \r\n", RxBufferPtr_0[i]); 
        xil_printf("Byte_0 = %lu \r\n", byte0);
        xil_printf("Byte_1 = %lu \r\n", byte1);  
        xil_printf("Byte_2 = %lu \r\n", byte2);  
        xil_printf("Byte_3 = %lu \r\n", byte3);
        if(TxBufferPtr_0[i] != RxBufferPtr_0[i]){
            status_1 = XST_FAILURE;
        }
    }

    xil_printf("status_1 == %i",status_1);
    for(int i = 0 ; i < 10; i++){xil_printf("++++++++++++++++++++++++++++++\r\n");}

    int status_2 = XST_SUCCESS;
    for(int i = USize.Output_1_Size ; i > -1 ; i--){
        u8 byte0 = (RxBufferPtr_1[i] >> 0) & 0xFF;  // Extract Byte0
        u8 byte1 = (RxBufferPtr_1[i] >> 8) & 0xFF;  // Extract Byte1
        u8 byte2 = (RxBufferPtr_1[i] >> 16) & 0xFF; // Extract Byte2
        u8 byte3 = (RxBufferPtr_1[i] >> 24) & 0xFF; // Extract Byte3
        xil_printf("Tx val = %lu ||", TxBufferPtr_0[i]);
        xil_printf("Rx_2 val = %i \r\n", RxBufferPtr_1[i]); 
        xil_printf("Byte_0 = %lu \r\n", byte0);
        xil_printf("Byte_1 = %lu \r\n", byte1);  
        xil_printf("Byte_2 = %lu \r\n", byte2);  
        xil_printf("Byte_3 = %lu \r\n", byte3);     
        if(TxBufferPtr_0[i] != RxBufferPtr_1[i]){
            status_2 = XST_FAILURE;
        }
    }

    if(status_1 != XST_SUCCESS || status_2 != XST_SUCCESS){
        xil_printf("Vals not consistent \r\n");
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}


