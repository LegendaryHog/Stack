#include "Assembler.h"

#define NO_CMD -1
#define _BREAK_IF_ if (*ptr == '\0'){breaking = 1; break;}
#define ass assert
#define CMD_NUM 6
#define MAX_LEN_CMD 5

enum {
    push,
    pop,
    sub,
    vdv,
    mul,
    hlt,
};

char cmd_name [CMD_NUM][MAX_LEN_CMD] = {
    "push",
    "pop",
    "sub",
    "vdv",
    "mul",
    "hlt",
};

FILE* logfile;

char* From_File_to_buffer (size_t* ptr_buffsize)
{
    logfile = fopen ("logfile.txt", "w");

    FILE* buffer = fopen ("cmdfile.txt", "r");
    ass (buffer != NULL);

    fseek(buffer, 0, SEEK_SET);
    long start_of_File = ftell (buffer);

    fseek(buffer, 0, SEEK_END);
    long end_of_File = ftell (buffer);

    fseek(buffer, 0, SEEK_SET);
    long size_File = end_of_File - start_of_File;

    char* ptrBuff = (char*) calloc (size_File + 1, sizeof (char));
    ass (ptrBuff != NULL);

    *ptr_buffsize = fread (ptrBuff, sizeof (char), size_File, buffer);

    fclose (buffer);

    *(ptrBuff + *ptr_buffsize + 1) = '\0';

    ptrBuff = (char*) realloc (ptrBuff, *ptr_buffsize + 1);
    ass (ptrBuff != NULL);

    return (char*)ptrBuff;
}

char* Buffer_to_Code (char* ptrbuff, size_t buffsize)
{
    char cmd = -1;
    size_t num_of_line = 0;
    int breaking = 0;


    char* code = (char*) calloc (10 * buffsize, sizeof (char));
    char* ptr = ptrbuff;
    char* code_copy = code;

    while (breaking == 0)
    {
        _BREAK_IF_
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
        {
            if (*ptr == '\n')
            {
                num_of_line++;
            }
            ptr++;
        }

        _BREAK_IF_
        for (int i = 0; i < CMD_NUM; i++)
        {
            if (strncmp (cmd_name[i], ptr, strlen(cmd_name[i])) == 0)
            {
                cmd = i;
            }
        }
        if (cmd == NO_CMD)
        {
            fprintf (logfile, "No command in line %zd or incorrect input\n", num_of_line + 1);
            return 0;
        }
        ptr += strlen (cmd_name[cmd]);
        switch (cmd) {
            case push:
                *code = cmd;

                code++;
                if (*ptr != ' ' && *ptr != '\t')
                {
                    fprintf (logfile, "No space between command and argument of push in line %zd\n", num_of_line + 1);
                    return NULL;
                }
                _BREAK_IF_
                while (*ptr == ' ' || *ptr == '\t')
                {
                    ptr++;
                }
                _BREAK_IF_

                *((double*)code) = Arg_Scan (ptr, num_of_line);

                printf ("%lf\n", *((double*)code));

                code += sizeof(double);

                while ((*ptr >= '0' && *ptr <= '9') || *ptr == '.')
                {
                    ptr++;
                }

                while (*ptr == ' ' || *ptr == '\t')
                {
                    ptr++;
                }

                _BREAK_IF_
                if (*ptr != '\n')
                {
                    fprintf (logfile, "Symbols after argument of push in line %zd\n", num_of_line + 1);
                    return NULL;
                }
                break;
            default:
                *code = cmd;

                printf ("%s\n", cmd_name[*code]);

                code++;

                while (*ptr == ' ' || *ptr == '\t')
                {
                    ptr++;
                }
                _BREAK_IF_
                if (*ptr != '\n')
                {
                    fprintf (logfile, "Symbols after %s in line %zd\n", cmd_name[cmd], num_of_line + 1);
                    return NULL;
                }
                break;
            }
    }
    fclose (logfile);

    return code_copy;
}

double pow_10 (int power)
{
    double res = 1;

    if (power > 0)
    {
        for (int i = 0; i < power; i++)
        {
            res *= 10;
        }
    }

    else if (power < 0)
    {
        for (int i = 0; i > power; i--)
        {
            res /= 10;
        }
    }

    return res;
}

double Arg_Scan (char* ptrarg, size_t num_of_line)
{
    int dot_pos = 0;
    double arg = 0;

    printf ("Argscan\n");

    while (ptrarg[dot_pos] >= '0' && ptrarg[dot_pos] <= '9')
    {
        dot_pos++;
    }

    if (ptrarg[dot_pos] != '.' && ptrarg[dot_pos] && ptrarg[dot_pos] != ' ' && ptrarg[dot_pos] != '\t' && ptrarg[dot_pos] != '\n' && ptrarg[dot_pos] != '\0')
    {
        fprintf (logfile, "Fucking symbols in argument on line %zd\n", num_of_line + 1);
        arg = 0;
    }
    else
    {
        while (*ptrarg != ' ' && *ptrarg != '\t' && *ptrarg != '\0' && *ptrarg != '\n')
        {
            arg += (*ptrarg - '0') * pow_10 (dot_pos - 1);
            ptrarg++;
            dot_pos--;
        }
    }

    return arg;
}

int Disassembler (char* code)
{
    int end = 0;

    FILE* Text = fopen ("text_code.txt", "w");

    while (end == 0)
    {
        switch (*code) {
            case push:
                printf ("%s ", cmd_name[*code]);
                fprintf (Text, "%s ", cmd_name[*code]);
                code++;
                printf ("%lf\n", *((double*)code));
                fprintf (Text, "%lf\n", *((double*)code));
                code += sizeof (double);
                break;
            case hlt:
                printf ("%s ", cmd_name[*code]);
                fprintf (Text, "%s\n", cmd_name[*code]);
                end = 1;
                code++;
                break;
            default:
                printf ("%s ", cmd_name[*code]);
                fprintf (Text, "%s\n", cmd_name[*code]);
                code++;
                break;
        }
    }
    fclose (Text);

    return 0;
}


