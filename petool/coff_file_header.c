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
    printf("    Machine: %s\n", machine_to_string(coff_file_header->Machine));
    printf("    NumberOfSections: %d\n", coff_file_header->NumberOfSections);
    printf("    TimeDateStamp: %s", ctime((time_t *)&coff_file_header->TimeDateStamp));
    printf("    PointerToSymbolTable: %d\n", coff_file_header->PointerToSymbolTable);
    printf("    NumberOfSymbols: %d\n", coff_file_header->NumberOfSymbols);
    printf("    SizeOfOptionalHeader: %d\n", coff_file_header->SizeOfOptionalHeader);
    printf("    Characteristics: %s\n", characteristics_to_string(coff_file_header->Characteristics));
}

#define CASE_RETURN(TAG) case TAG: return #TAG;
#define CASE_RETURN_2(TAG1, TAG2) case TAG1: return #TAG1 ", " #TAG2;

const char * machine_to_string(machine_t machine)
{
    switch (machine)
    {
        CASE_RETURN(IMAGE_FILE_MACHINE_UNKNOWN)
        CASE_RETURN(IMAGE_FILE_MACHINE_ALPHA)
        CASE_RETURN_2(IMAGE_FILE_MACHINE_ALPHA64, IMAGE_FILE_MACHINE_AXP64)
        CASE_RETURN(IMAGE_FILE_MACHINE_AM33)
        CASE_RETURN(IMAGE_FILE_MACHINE_AMD64)
        CASE_RETURN(IMAGE_FILE_MACHINE_ARM)
        CASE_RETURN(IMAGE_FILE_MACHINE_ARMNT)
        CASE_RETURN(IMAGE_FILE_MACHINE_ARM64)
        CASE_RETURN(IMAGE_FILE_MACHINE_CEE)
        CASE_RETURN(IMAGE_FILE_MACHINE_CEF)
        CASE_RETURN(IMAGE_FILE_MACHINE_EBC)
        CASE_RETURN(IMAGE_FILE_MACHINE_I386)
        CASE_RETURN(IMAGE_FILE_MACHINE_IA64)
        CASE_RETURN(IMAGE_FILE_MACHINE_M32R)
        CASE_RETURN(IMAGE_FILE_MACHINE_MIPS16)
        CASE_RETURN(IMAGE_FILE_MACHINE_MIPSFPU)
        CASE_RETURN(IMAGE_FILE_MACHINE_MIPSFPU16)
        CASE_RETURN(IMAGE_FILE_MACHINE_POWERPC)
        CASE_RETURN(IMAGE_FILE_MACHINE_POWERPCFP)
        CASE_RETURN(IMAGE_FILE_MACHINE_R10000)
        CASE_RETURN(IMAGE_FILE_MACHINE_R3000)
        CASE_RETURN(IMAGE_FILE_MACHINE_R4000)
        CASE_RETURN(IMAGE_FILE_MACHINE_SH3)
        CASE_RETURN(IMAGE_FILE_MACHINE_SH3DSP)
        CASE_RETURN(IMAGE_FILE_MACHINE_SH3E)
        CASE_RETURN(IMAGE_FILE_MACHINE_SH4)
        CASE_RETURN(IMAGE_FILE_MACHINE_SH5)
        CASE_RETURN(IMAGE_FILE_MACHINE_THUMB)
        CASE_RETURN(IMAGE_FILE_MACHINE_TRICORE)
        CASE_RETURN(IMAGE_FILE_MACHINE_WCEMIPSV2)
    }
    return "UNKNOWN";
}

#define FLAG_CASE(TAG)                                          \
if (value & TAG) {                                              \
    i += sprintf_s(buf + i, buf_size - i, "%s ", #TAG);         \
}                                                               \

const char * characteristics_to_string(characteristics_t value)
{
    size_t max_len = 5000;
    size_t buf_size = 5001;
    size_t i = 0;
    char buf[5001];

    FLAG_CASE(IMAGE_FILE_RELOCS_STRIPPED)
    FLAG_CASE(IMAGE_FILE_EXECUTABLE_IMAGE)
    FLAG_CASE(IMAGE_FILE_LINE_NUMS_STRIPPED)
    FLAG_CASE(IMAGE_FILE_LOCAL_SYMS_STRIPPED)
    FLAG_CASE(IMAGE_FILE_AGGRESIVE_WS_TRIM)
    FLAG_CASE(IMAGE_FILE_LARGE_ADDRESS_AWARE)
    FLAG_CASE(IMAGE_FILE_BYTES_REVERSED_LO)
    FLAG_CASE(IMAGE_FILE_32BIT_MACHINE)
    FLAG_CASE(IMAGE_FILE_DEBUG_STRIPPED)
    FLAG_CASE(IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP)
    FLAG_CASE(IMAGE_FILE_NET_RUN_FROM_SWAP)
    FLAG_CASE(IMAGE_FILE_SYSTEM)
    FLAG_CASE(IMAGE_FILE_DLL)
    FLAG_CASE(IMAGE_FILE_UP_SYSTEM_ONLY)
    FLAG_CASE(IMAGE_FILE_BYTES_REVERSED_HI)

    return buf;
}