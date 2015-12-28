#include <stdio.h>

void parse_config(const char *fname)
{
    FILE *infile = NULL;
    fopen_s(&infile, fname, "r");





    fclose(infile);
}