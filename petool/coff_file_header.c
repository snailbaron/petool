#include "coff_file_header.h"
#include "error.h"
#include <time.h>



bool read_coff_file_header(FILE *infile, coff_file_header_t *coff_file_header)
{
    if (fread(coff_file_header, sizeof(coff_file_header_t), 1, infile) < 1) {
        set_error("Failed to read COFF file header");
        return false;
    }
    return true;
}

void print_coff_file_header(const coff_file_header_t *coff_file_header)
{
    printf("COFF File Header:\n");
    printf("Machine: 0x%x\n", coff_file_header->Machine);
    printf("NumberOfSections: %d\n", coff_file_header->NumberOfSections);
    printf("TimeDateStamp: %s\n", ctime((time_t *)&coff_file_header->TimeDateStamp));
    printf("PointerToSymbolTable: %d\n", coff_file_header->PointerToSymbolTable);
    printf("NumberOfSymbols: %d\n", coff_file_header->NumberOfSymbols);
    printf("SizeOfOptionalHeader: %d\n", coff_file_header->SizeOfOptionalHeader);
    printf("Characteristics: 0x%x\n", coff_file_header->Characteristics);
}