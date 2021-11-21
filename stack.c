#include "stack.h"

#define STK_L_CAN_DIE 1<<0
#define STK_R_CAN_DIE 1<<1
#define DATA_L_CAN_DIE 1<<2
#define DATA_R_CAN_DIE 1<<3
#define STK_UFLOW 1<<4
#define STK_OFLOW 1<<5
#define SIZE_LESS_NULL 1<<6
#define CAP_LESS_STRT_CAP 1<<7
#define DATA_P_NULL 1<<8
#define LOG_P_NULL 1<<9
#define HASH_ERR 1<<10
#define DOUBLE_DTOR 1<<11
#define ERR_NUM 12

static enum{
    UP,
    DOWN
};

char errors_name [][VLG] = {
    {"stack left canary corruption"},  //(0)
    {"stack right canary corruption"}, //(1)
    {"data left canary corruption"},   //(2)
    {"data right canary corruption"},  //(3)
    {"stack underflow"},               //(4)
    {"stack overflow"},                //(5)
    {"size less null"},                //(6)
    {"capacity less start capacity"},  //(7)
    {"!!! DATA POINTER IS NULL !!!"},  //(8)
    {"stk->logfile pointer is null"},  //(9)
    {"hash check error"},              //(10)
    {"!!! DOUBLE DTOR !!!"},           //(11)
    {"Stack pointer is null"}          //(12)
};

void ass (int expression, FILE* logfile)
{
    if (expression == 0)
    {
        fprintf (logfile, "\n do meow from line %d from function %s", __LINE__, __PRETTY_FUNCTION__ );
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//Hash
static long long Hash_Calc (stack* const stk)
{
    NULL_PTR_EXIT
    Stack_Check (stk);

    long long hash = 0;

    for (int i = 0; i < sizeof (stack); i++)
    {
        if (!(i >= (char*)&(stk->hash) - (char*)stk && i < (char*)&(stk->canary2) - (char*)stk))
        {
            hash += (i + 1) * (*((char*)stk + i));
        }
    }

    for (long long i = 0; i < stk->capacity * stk->type_s + 2 * sizeof (canary_t); i++)
    {
        hash += (i + 1) * (*((char*)stk->data - sizeof (canary_t) + i));
    }

    Stack_Check (stk);
    return hash;
}

static int Hash_Check (stack* const stk)
{
    Stack_Check (stk);
    NULL_PTR_EXIT

    long long hash = Hash_Calc (stk);

    if (hash != stk->hash)
    {
        stk->error |= HASH_ERR;
        for (int i = 0; i < GOD_LOVE_TRIPLE; i++)
        {
            fprintf (stk->logfile, "\n !!! HASH CHECK ERROR !!! stk->hash = %lld hash = %lld\n\n", stk->hash, hash);
        }
        Stack_Check (stk);
        return 0;
    }
    else
    {
        Stack_Check (stk);
        return 1;
    }
}

static void Hash_Up (stack* stk)
{
    stk->hash = Hash_Calc (stk);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

int Stack_Ctor (stack* stk, size_t type_ass, void (*fprint_elem) (FILE* file, void* ptrelem))
{
    if (stk == NULL)
    {
        printf ("Argument of Stack_Ctor () is null pointer\n ");
        return 0;
    }
    stk->type_s = type_ass;
    stk->fprint_elem = fprint_elem;

    stk->canary1 = 0xBE31AB;
    stk->capacity = CAPACITY_0;
    stk->size = 0;
    stk->data = (void*) ((char*) calloc (CAPACITY_0 * stk->type_s + 2 * sizeof (canary_t), sizeof (char)) + sizeof (canary_t));
    DATACANARY1 = 0xD1CC0C;    //left canary of stack
    DATACANARY2 = 0xC0CA0;     //right canary of data
    stk->logfile = fopen ("logfile.txt", "w");
    stk->error = 0;
    stk->canary2 = 0xBADDED;

    stk->hash = Hash_Calc (stk);

    Hash_Check (stk);
    Stack_Check (stk);

    return 1;
}

void Stack_Dtor (stack* stk)
{
    Stack_Check (stk);

    NULL_PTR_EXIT
    if (stk->data == POISON)
    {
        stk->error |= DOUBLE_DTOR;
        Stack_Check (stk);
    }
    else if (stk->data == NULL)
    {
        Stack_Check (stk);
    }
    else
    {
        fclose (stk->logfile);
        free ((char*)stk->data - sizeof (canary_t));
        stk->data = POISON;
    }
}

int Stack_Push (stack* stk, void* ptrpush)
{
    NULL_PTR_EXIT

    Hash_Check (stk);
    Stack_Check (stk);
    Stack_Resize(stk, UP);

    for (int i = 0; i < stk->type_s; i++)
    {
        *((char*)stk->data + stk->size * stk->type_s + i) = *((char*)ptrpush + i);
    }

    stk->size++;
    Hash_Up (stk);
    Stack_Check (stk);
    return 1;
}

int Stack_Resize (stack* stk, const int  mode)
{
    Hash_Check (stk);
    Stack_Check (stk);

    NULL_PTR_EXIT

    long long start_capacity = stk->capacity;

    if ((stk->size == stk->capacity && mode == UP) || (stk->size == stk->capacity/2 - 5 && stk->capacity > CAPACITY_0 && mode == DOWN))
    {
        if (mode == UP)
        {
            stk->capacity *= 2;
        }
        else
        {
            stk->capacity /= 2;
        }
        stk->data = (void*) ((char*)realloc ((char*)stk->data - sizeof (canary_t), stk->capacity * stk->type_s + 2 * sizeof (canary_t)) + sizeof (canary_t));
        if ((char*)(stk->data) - sizeof (canary_t) == NULL)
        {
            fprintf (stk->logfile, " !!!Stack Resize error!!! :\ncapacity = %lld;\nsize = %lld\nGo and buy new laptop.\n", stk->capacity, stk->size);
            Stack_Check (stk);
            Hash_Up (stk);
            return 0;
        }
        else
        {

            DATACANARY2 = *((canary_t*)((char*)stk->data + start_capacity * stk->type_s));
            *((canary_t*)((char*)stk->data + start_capacity * stk->type_s)) = 0;
            if (mode == UP)
            {
                for (int i = 0; i < stk->capacity/2 * stk->type_s; i++)
                {
                    *((char*)stk->data + stk->capacity/2 * stk->type_s + i) = 0;
                }
            }
            Stack_Check (stk);
            Hash_Up (stk);
            return 1;
        }
    }
    else
    {
        Stack_Check (stk);
        return 0;
    }
}

int Stack_Pop (stack* stk, void* ptrpop)
{
    NULL_PTR_EXIT

    Hash_Check (stk);
    Stack_Check (stk);

    if (stk->size == 0)
    {
        stk->error |= STK_UFLOW;
        Stack_Check (stk);
        return 0;
    }
    Stack_Resize (stk, DOWN);
    stk->size--;
    for (size_t i = 0; i < stk->type_s; i++)
    {
        *((char*)ptrpop + i) = *((char*)stk->data + stk->size * stk->type_s + i);
    }
    for (size_t i = 0; i < stk->type_s; i++)
    {
        *((char*)stk->data + stk->size * stk->type_s + i) = 0;
    }

    Hash_Up (stk);

    return 0;
}

int Stack_Dump (const stack* const stk)
{
    if (stk == NULL)
    {
        fprintf (stk->logfile, "Stack does not exist\n");
        return 0;
    }

    fprintf (stk->logfile, "Stack [%p]", stk);
    if (stk->error > 0)
    {
        fprintf (stk->logfile, "Stack is BAD:\n");

        for (int i = 0; i < ERR_NUM; i++)
        {
            if (stk->error & 1<<i)
            {
                fprintf (stk->logfile, "%s;\n", errors_name[i]);
            }
        }
    }
    else
    {
        fprintf (stk->logfile, "Stack is Ok.\n");
    }
    fprintf (stk->logfile, "{\n    canary1 = %0X\n    hash = %lld\n    capacity = %lld\n    size = %lld\n    ", stk->canary1, stk->hash, stk->capacity, stk->size);
    fprintf (stk->logfile, "data [%p]\n    {\n        ", stk->data);
    if (stk->data > POISON);
    {
        fprintf (stk->logfile, "datacanary1 = %0X\n        ", DATACANARY1);
        if (stk->fprint_elem != NULL)
        {
            if (stk->capacity <= LOW_CAPACITY)
            {
                for (size_t i = 0; i < stk->capacity; i++)
                {
                    if (i < stk->size)
                    {
                        fprintf (stk->logfile, "*[%zd] = ", i);
                    }
                    else
                    {
                        fprintf (stk->logfile, " [%zd] = ", i);
                    }
                    stk->fprint_elem (stk->logfile, (char*)stk->data + i * stk->type_s);
                    fprintf (stk->logfile, "\n        ");
                }
            }
            else
            {
                for (long long i = 0; i < START_NUM; i++)
                {
                    if (i < stk->size)
                    {
                        fprintf (stk->logfile, "*[%zd] = ", i);
                    }
                    else
                    {
                        fprintf (stk->logfile, "*[%zd] = ", i);
                    }
                    stk->fprint_elem (stk->logfile, (char*)stk->data + i * stk->type_s);
                    fprintf (stk->logfile, "\n        ");
                }
                fprintf (stk->logfile, ".\n        .\n        .\n        ");

                for (long long i = END_NUM; i >= END_NUM && i < stk->capacity; i++)
                {
                    if (i < stk->size)
                    {
                        fprintf (stk->logfile, "*[%zd] = ", i);
                    }
                    else
                    {
                        fprintf (stk->logfile, " [%zd] = ", i);
                    }
                    stk->fprint_elem (stk->logfile, (char*)stk->data + i * stk->type_s);
                    fprintf (stk->logfile, "\n        ");
                }
            }
        }
        else
        {
            fprintf (stk->logfile, "\n        If you want to see elements of your stack,\n");
            fprintf (stk->logfile, "        you must write function that print element of your stack:\n\n         void (name) (FILE* file, void* ptrelem)\n\n");
            fprintf (stk->logfile, "        and give pointer on it in Stack_Ctor in third argument\n");
            fprintf (stk->logfile, "        do not be baka and write it.\n\n");
        }
        fprintf (stk->logfile, "        datacanary2 = %0X\n    }\n", DATACANARY2);
    }
    fprintf (stk->logfile, "    canary2 = %0X\n}\n", stk->canary2);
    return 0;
}

int StaCkok (stack* const stk)
{
    NULL_PTR_EXIT

    stk->error |= STK_L_CAN_DIE && (stk->canary1 != 0xBE31AB);
    stk->error |= STK_R_CAN_DIE && (stk->canary2 !=  0xBADDED);
    stk->error |= DATA_L_CAN_DIE && (DATACANARY1 != 0xD1CC0C);
    stk->error |= DATA_R_CAN_DIE && (DATACANARY1 != 0xD1CC0C);
    stk->error |= STK_OFLOW && (stk->size > stk->capacity);
    stk->error |= SIZE_LESS_NULL && (stk->size < 0);
    stk->error |= CAP_LESS_STRT_CAP && (stk->capacity < CAPACITY_0);
    stk->error |= DATA_P_NULL && (stk->data == NULL);
    stk->error |= LOG_P_NULL && (stk->logfile == NULL);

    if (stk->error > 0)
    {
        return 0;
    }
    return 1;
}

void Stack_Check (const stack* const stk)
{
    if (StaCkok (stk) == 0)
    {
        Stack_Dump (stk);
    }
}

void Hex_To_You (FILE* file, void* ptrelem, size_t type_ass)
{
    for (size_t i = 0; i < type_ass; i++)
    {
        fprintf (file, "%0X", *((char*)ptrelem + i));
    }
}

