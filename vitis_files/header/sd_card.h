#ifndef SDCARD_H
#define SDCARD_H

#include <xil_types.h>
#include <xil_printf.h>
#include <stdlib.h>
#include <ff.h>

int SD_Init(void);
int SD_Eject(void);
int ReadFile(char *FileName, UINTPTR DestinationAddress);
int WriteFile(char *FileName, size_t size, UINTPTR SourceAddress);
int write_to_weights_from_file(char *FileName, long BaseAddress) ;

#endif