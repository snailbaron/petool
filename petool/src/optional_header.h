#ifndef OPTIONAL_HEADER_H
#define OPTIONAL_HEADER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    uint32_t VirtualAddress;
    uint32_t Size;
} data_directory_t;

// Optional Header: PE32 format
typedef struct {
    // Optional Header Standard Fields (PE32)
    uint16_t Magic;
    uint8_t  MajorLinkerVersion;
    uint8_t  MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
    uint32_t BaseOfData;          // in PE32 only

    // Optional Header Windows-Specific Fields (PE32)
    uint32_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint32_t SizeOfStackReserve;
    uint32_t SizeOfStackCommit;
    uint32_t SizeOfHeapReserve;
    uint32_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;

    data_directory_t DataDirectory[];
} optional_header_pe32_t;

// Optional Header: PE32+ format
typedef struct {
    // Optional Header Standard Fields (PE32+)
    uint16_t Magic;
    uint8_t  MajorLinkerVersion;
    uint8_t  MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;

    // Optional Header Windows-Specific Fields (PE32+)
    uint64_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint64_t SizeOfStackReserve;
    uint64_t SizeOfStackCommit;
    uint64_t SizeOfHeapReserve;
    uint64_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;

    data_directory_t DataDirectory[];
} optional_header_pe32_plus_t;

// Optional Header: can have PE32 or PE32+ format
typedef union {
    uint16_t Magic;
    optional_header_pe32_t pe32;
    optional_header_pe32_plus_t pe32_plus;
} optional_header_t;

// Optional Header: Magic Number
typedef enum {
    OPTIONAL_HEADER_MAGIC_PE32      = 0x10B,
    OPTIONAL_HEADER_MAGIC_PE32_PLUS = 0x20B,
} optional_header_magic_t;



bool read_optional_header(FILE *infile, optional_header_t *header);
void print_optional_header(optional_header_t *header);


#endif
