#include "stack.h"

void fprint_double (FILE* file, void* ptrdouble);
void fprint_int (FILE* file, void* ptrint);
void Stack_Push_d (stack* stk, double push);
double Stack_Pop_d (stack* stk);

int main (void)
{
    stack stk = {};
    stack stk1 = {};
    Stack_Ctor (&stk1, "int",  sizeof (int), fprint_int);
    Stack_Ctor (&stk, "double", sizeof(double), fprint_double);

    for (int i = 0; i < 20; i++)
    {
        Stack_Push_d (&stk, 10);
    }
    double a = 0;
    Stack_Dump (&stk);

    printf ("%lf\n", Stack_Pop_d (&stk));
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
void Stack_Push_d (stack* stk, double push)
{
    Stack_Push (stk, &push);
}
double Stack_Pop_d (stack* stk)
{
    double pop = 0;
    Stack_Pop (stk, &pop);
    return pop;
}





