#ifndef COFF_FILE_HEADER_H
#define COFF_FILE_HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
} coff_file_header_t;

bool read_coff_file_header(FILE *infile, coff_file_header_t *coff_file_header);
void print_coff_file_header(const coff_file_header_t *coff_file_header);


#endif
