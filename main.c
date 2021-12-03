#include "Assembler.h"

int main (void)
{
    size_t buffsize = 0;

    char* ptrbuff = From_File_to_buffer (&buffsize);

    char* code = Buffer_to_Code (ptrbuff, buffsize);

    FILE* Code = fopen ("machine_code.bin", "wb");

    if (code != NULL)
    {
        Disassembler (code);
    }
    else
    {
        printf ("assembler error\n");
        return 0;
    }

    fwrite (code, sizeof (char), sizeof (code), Code);

    fclose (Code);
    free (code);

    return 0;
}
