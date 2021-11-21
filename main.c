#include "stack.h"

void fprint_double (FILE* file, void* ptrdouble);

int main (void)
{
    stack stk = {0};

    Stack_Ctor (&stk, sizeof(double), NULL) ;


    for (int i = 0; i < 2048; i++)
    {
        double x = 10;
        Stack_Push (&stk, &x);
    }
    double a = 0;
    Stack_Dump (&stk);
    Stack_Pop (&stk, &a);
    printf ("%lf\n", a);
    Stack_Dump (&stk);
    Stack_Dtor (&stk);

    return 0;
}

void fprint_double (FILE* file, void* ptrdouble)
{
    fprintf (file, "%lf", *((double*)ptrdouble));
}


