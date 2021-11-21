#include "stack.h"

void fprint_double (FILE* file, void* ptrdouble);
void fprint_int (FILE* file, void* ptrint);

int main (void)
{
    stack stk = {0};
    stack stk1 = {};
    Stack_Ctor (&stk1, "int\0",  sizeof (int), fprint_int);
    Stack_Ctor (&stk, "double\0", sizeof(double), fprint_double);

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

    int b = 15;
    Stack_Push (&stk1, &b);
    Stack_Dump (&stk1);
    Stack_Pop (&stk1, NULL);
    Stack_Dump (&stk1);
    Stack_Dtor (&stk1);

    return 0;
}

void fprint_double (FILE* file, void* ptrdouble)
{
    fprintf (file, "%lf", *((double*)ptrdouble));
}
void fprint_int (FILE* file, void* ptrint)
{
    fprintf (file, "%d", *((int*)ptrint));
}





