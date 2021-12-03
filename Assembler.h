#ifndef ASS
#define ASS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

char* From_File_to_buffer (size_t* ptr_buffsize);

char* Buffer_to_Code (char* ptrbuff, size_t buffsize);

double pow_10 (int power);

double Arg_Scan (char* ptrarg, size_t num_of_line);

int Disassembler (char* code);

#endif

