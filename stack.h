#ifndef STACK_INCLUDED
#define STACK_INCLUDED

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define POISON 1000-7

#define data_t double
#define _FMT_ "%lf"
#define canary_t long long

#define LOW_CAPACITY 48
#define START_NUM 24
#define END_NUM stk->capacity - 16
#define VLG 34
#define NULL_PTR_EXIT if (stk == NULL){return 0;}
#define GOD_LOVE_TRIPLE 3


#define DATACANARY1 *((canary_t*)((char*)stk->data - sizeof (canary_t)))
#define DATACANARY2 *((canary_t*)((char*)stk->data + stk->capacity * stk->type_s))

#define CAPACITY_0 8

typedef struct {
    long long canary1;
    long long size;
    long long capacity;
    void* data;
    long long hash;
    size_t type_s;
    void (*fprint_elem) (FILE* file, void* ptrelem);
    FILE* logfile;
    unsigned int error;
    long long canary2;
} stack;

//--------------------------------------------
//Hash

/*static long long Hash_Calc (stack* const stk);

static void Hash_Up (stack* stk);

static int Hash_Check (const stack* const stk);*/

//--------------------------------------------

int Stack_Ctor (stack* stk, size_t type_ass, void (*fprint) (FILE* file, void* ptrelem));

void Stack_Dtor (stack* stk);

int Stack_Push (stack* stk, void* ptrpush);

int Stack_Resize (stack* stk, const int mode);

int Stack_Pop (stack* stk, void* ptrpop);

void ass (int expression, FILE* logfile);

int Stack_Dump (const stack* const stk);

int StaCkok (stack* const stk);

void Stack_Check (const stack* const stk);

void Hex_To_You (FILE* file, void* ptrelem, size_t type_ass);

#endif

