#include "sd_card.h"
#include <string.h>
#include <xil_printf.h>
#include <xil_types.h>

FATFS fatfs;

int SD_Init(void)
{
	FRESULT Res;
	TCHAR *Path = "1:/";
	Res = f_mount(&fatfs,Path,0);
	if (Res != FR_OK) {
		xil_printf(" ERROR : f_mount returned %d\r\n", Res);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int SD_Eject(void)
{
	FRESULT Res;
	TCHAR *Path = "0:/";
	Res = f_unmount(Path);
	if (Res != FR_OK) {
		xil_printf(" ERROR : f_unmount returned %d\r\n", Res);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int ReadFile(char *FileName, UINTPTR DestinationAddress)
{   
    FRESULT Res;
    static FIL fil;
    FSIZE_t file_size;
    UINT NumBytesRead;
    
    Res = f_open(&fil, FileName, FA_READ);
	if (Res) {
        xil_printf("Exited f_open with %d\r\n", Res);
		return XST_FAILURE;
	}
    
    file_size = f_size(&fil);
    xil_printf("File size is == %lu\r\n", file_size);
    
    Res = f_lseek(&fil, 0);
	if (Res) {
        xil_printf("Exited f_lseek with %d\r\n", Res);
		return XST_FAILURE;
	}

    Res = f_read(&fil, (void*)DestinationAddress, file_size,
			&NumBytesRead);
	if (Res) {
        xil_printf("Exited f_read with %d\r\n", Res);
		return XST_FAILURE;
	}

    Res = f_close(&fil);
	if (Res) {
        xil_printf("Exited f_close with %d\r\n", Res);
		return XST_FAILURE;
	}

    return XST_SUCCESS;
}

int WriteFile(char *FileName, size_t size, UINTPTR SourceAddress){
    FRESULT Res;
    static FIL fil;
    UINT NumBytesWritten;

    Res = f_open(&fil, FileName, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
        xil_printf("Exited f_open with %d\r\n", Res);
		return XST_FAILURE;
	}

    Res = f_lseek(&fil, 0);
	if (Res) {
        xil_printf("Exited f_lseek with %d\r\n", Res);
		return XST_FAILURE;
	}

    Res = f_write(&fil, (const void*)SourceAddress, size,
			&NumBytesWritten);
	if (Res) {
        xil_printf("Exited f_write with %d\r\n", Res);
		return XST_FAILURE;
	}

    Res = f_close(&fil);
	if (Res) {
        xil_printf("Exited f_close with %d\r\n", Res);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int write_to_weights_from_file(char *FileName, long BaseAddress) {
    FIL fil;        /* File object */
    char line[10]; /* Line buffer */
    FRESULT Res;     /* FatFs return code */
    u32 weight = 0;
    u32 read_weight;
    UINTPTR BasedAdress = (UINTPTR) BaseAddress;
    int offset = 0;

    /* Open a text file */
    Res = f_open(&fil, FileName, FA_READ);
    if (Res) {
        xil_printf("Exited f_open with %d\r\n", Res);
		return XST_FAILURE;
	}
    xil_printf("RuntimeWeights: File open ok\r\n");
    
    /* Read every line, convert to HEX, then load into memory */
    while (f_gets(line, sizeof line, &fil)) {
        weight = (u32)strtoul(line,NULL,16);
        // xil_printf("addr = %lu\r\n", BasedAdress+offset);
        Xil_SecureOut32(BasedAdress + offset, weight);
        offset += 4;
    }
     xil_printf("RuntimeWeights: File read ok\r\n");

    //Rewind the file
    Res = f_lseek(&fil, 0);
	if (Res) {
        xil_printf("Exited f_lseek with %d\r\n", Res);
		return XST_FAILURE;
	}
     xil_printf("RuntimeWeights: File seek ok\r\n");

    //Check if weight loaded in
    //Xil_SecureOut32 checks twice to see if the value was written,
    //to be sure cross check again.
    offset = 0;
    while (f_gets(line, sizeof line, &fil)) {
        weight = (u32)strtol(line,NULL,16);
        read_weight = (u32) Xil_In32(BasedAdress + offset);
        // xil_printf("read_weight = %lu\r\n",read_weight);
        if(weight != read_weight){
            xil_printf("Vals not consistend got %lu, expected %lu",read_weight,weight);
            return XST_FAILURE;
        }
        offset += 4;
    }
     xil_printf("RuntimeWeights: Weight check ok\r\n");
    
    xil_printf("n == %ld\r\n",weight);
    Res = f_close(&fil);
	if (Res) {
        xil_printf("Exited f_close with %d\r\n", Res);
		return XST_FAILURE;
	}
    xil_printf("RuntimeWeights: File close ok\r\n");

	return XST_SUCCESS;
}