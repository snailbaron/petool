#include "optional_header.h"
#include "error.h"

bool read_optional_header(FILE *infile, optional_header_t *header)
{
    uint16_t magic;
    if (fread(&magic, 2, 1, infile) < 1) {
        set_error("Failed to read magic number from optional header");
        return false;
    }

    if (fseek(infile, -2, SEEK_CUR)) {
        set_error("Failed to set back after reading macic number from optional header");
        return false;
    }

    if (magic == OPTIONAL_HEADER_MAGIC_PE32) {
        fread(header, sizeof(optional_header_pe32_t), 1, infile);
    } else if (magic == OPTIONAL_HEADER_MAGIC_PE32_PLUS) {
        fread(header, sizeof(optional_header_pe32_plus_t), 1, infile);
    } else {
        set_error("Invalid magic in optional header");
        return false;
    }

    return true;
}

void print_optional_header(optional_header_t *header)
{
    if (header->Magic == OPTIONAL_HEADER_MAGIC_PE32) {
        optional_header_pe32_t *h = (optional_header_pe32_t *)header;

        printf("Optional Header:\n");
        printf("  Standard Fields:\n");
        printf("    Magic: 0x%x (PE32)\n", h->Magic);
        printf("    MajorLinkerVersion: %d\n", h->MajorLinkerVersion);
        printf("    MinorLinkerVersion: %d\n", h->MinorLinkerVersion);
        printf("    SizeOfCode: %d\n", h->SizeOfCode);
        printf("    SizeOfInitializedData: %d\n", h->SizeOfInitializedData);
        printf("    SizeOfUninitializedData: %d\n", h->SizeOfUninitializedData);
        printf("    AddressOfEntryPoint: %d\n", h->AddressOfEntryPoint);
        printf("    BaseOfCode: %d\n", h->BaseOfCode);
        printf("    BaseOfData: %d\n", h->BaseOfData);
        printf("  Windows-Specific Fields:\n");
        printf("    ImageBase: %d\n", h->ImageBase);
        printf("    SectionAlignment: %d\n", h->SectionAlignment);
        printf("    FileAlignment: %d\n", h->FileAlignment);
        printf("    MajorOperatingSystemVersion: %d\n", h->MajorOperatingSystemVersion);
        printf("    MinorOperatingSystemVersion: %d\n", h->MinorOperatingSystemVersion);
        printf("    MajorImageVersion: %d\n", h->MajorImageVersion);
        printf("    MinorImageVersion: %d\n", h->MinorImageVersion);
        printf("    MajorSubsystemVersion: %d\n", h->MajorSubsystemVersion);
        printf("    MinorSubsystemVersion: %d\n", h->MinorSubsystemVersion);
        printf("    Win32VersionValue: %d\n", h->Win32VersionValue);
        printf("    SizeOfImage: %d\n", h->SizeOfImage);
        printf("    SizeOfHeaders: %d\n", h->SizeOfHeaders);
        printf("    CheckSum: %d\n", h->CheckSum);
        printf("    Subsystem: %d\n", h->Subsystem);
        printf("    DllCharacteristics: %d\n", h->DllCharacteristics);
        printf("    SizeOfStackReserve: %d\n", h->SizeOfStackReserve);
        printf("    SizeOfStackCommit: %d\n", h->SizeOfStackCommit);
        printf("    SizeOfHeapReserve: %d\n", h->SizeOfHeapReserve);
        printf("    SizeOfHeapCommit: %d\n", h->SizeOfHeapCommit);
        printf("    LoaderFlags: %d\n", h->LoaderFlags);
        printf("    NumberOfRvaAndSizes: %d\n", h->NumberOfRvaAndSizes);

    } 
}