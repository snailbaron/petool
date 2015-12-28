#ifndef COFF_FILE_HEADER_H
#define COFF_FILE_HEADER_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// COFF File Header
typedef struct {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
} coff_file_header_t;

// COFF File Header: Machine
typedef enum {
    IMAGE_FILE_MACHINE_UNKNOWN          = 0,
    IMAGE_FILE_MACHINE_ALPHA            = 0x0184,  // Alpha_AXP, NOT IN SPEC
    IMAGE_FILE_MACHINE_ALPHA64          = 0x0284,  // ALPHA64, NOT IN SPEC
    IMAGE_FILE_MACHINE_AM33             = 0x01d3,
    IMAGE_FILE_MACHINE_AMD64            = 0x8664,  // AMD64 (K8)
    IMAGE_FILE_MACHINE_ARM              = 0x01c0,  // ARM Little-Endian
    IMAGE_FILE_MACHINE_ARMNT            = 0x01c4,  // ARM Thumb-2 Little-Endian
    IMAGE_FILE_MACHINE_AXP64            = IMAGE_FILE_MACHINE_ALPHA64,   // NOT IN SPEC
    IMAGE_FILE_MACHINE_ARM64            = 0xAA64,  // from spec
    IMAGE_FILE_MACHINE_CEE              = 0xC0EE,   // NOT IN SPEC
    IMAGE_FILE_MACHINE_CEF              = 0x0CEF,   // NOT IN SPEC
    IMAGE_FILE_MACHINE_EBC              = 0x0EBC,  // EFI Byte Code
    IMAGE_FILE_MACHINE_I386             = 0x014c,  // Intel 386.
    IMAGE_FILE_MACHINE_IA64             = 0x0200,  // Intel 64
    IMAGE_FILE_MACHINE_M32R             = 0x9041,  // M32R little-endian
    IMAGE_FILE_MACHINE_MIPS16           = 0x0266,  // MIPS
    IMAGE_FILE_MACHINE_MIPSFPU          = 0x0366,  // MIPS
    IMAGE_FILE_MACHINE_MIPSFPU16        = 0x0466,  // MIPS
    IMAGE_FILE_MACHINE_POWERPC          = 0x01F0,  // IBM PowerPC Little-Endian
    IMAGE_FILE_MACHINE_POWERPCFP        = 0x01f1,
    IMAGE_FILE_MACHINE_R10000           = 0x0168,  // MIPS little-endian, NOT IN SPEC
    IMAGE_FILE_MACHINE_R3000            = 0x0162,  // MIPS little-endian, 0x160 big-endian, NOT IN SPEC
    IMAGE_FILE_MACHINE_R4000            = 0x0166,  // MIPS little-endian
    IMAGE_FILE_MACHINE_SH3              = 0x01a2,  // SH3 little-endian
    IMAGE_FILE_MACHINE_SH3DSP           = 0x01a3,
    IMAGE_FILE_MACHINE_SH3E             = 0x01a4,  // SH3E little-endian, NOT IN SPEC
    IMAGE_FILE_MACHINE_SH4              = 0x01a6,  // SH4 little-endian
    IMAGE_FILE_MACHINE_SH5              = 0x01a8,  // SH5
    IMAGE_FILE_MACHINE_THUMB            = 0x01c2,  // ARM Thumb/Thumb-2 Little-Endian
    IMAGE_FILE_MACHINE_TRICORE          = 0x0520,  // Infineon, NOT IN SPEC
    IMAGE_FILE_MACHINE_WCEMIPSV2        = 0x0169,  // MIPS little-endian WCE v2
} machine_t;

// COFF File Header: Characteristics
typedef enum {
    IMAGE_FILE_RELOCS_STRIPPED          = 0x0001,  // Relocation info stripped from file.
    IMAGE_FILE_EXECUTABLE_IMAGE         = 0x0002,  // File is executable  (i.e. no unresolved external references).
    IMAGE_FILE_LINE_NUMS_STRIPPED       = 0x0004,  // Line nunbers stripped from file.
    IMAGE_FILE_LOCAL_SYMS_STRIPPED      = 0x0008,  // Local symbols stripped from file.
    IMAGE_FILE_AGGRESIVE_WS_TRIM        = 0x0010,  // Aggressively trim working set
    IMAGE_FILE_LARGE_ADDRESS_AWARE      = 0x0020,  // App can handle >2gb addresses
    IMAGE_FILE_BYTES_REVERSED_LO        = 0x0080,  // Bytes of machine word are reversed.
    IMAGE_FILE_32BIT_MACHINE            = 0x0100,  // 32 bit word machine.
    IMAGE_FILE_DEBUG_STRIPPED           = 0x0200,  // Debugging info stripped from file in .DBG file
    IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP  = 0x0400,  // If Image is on removable media, copy and run from the swap file.
    IMAGE_FILE_NET_RUN_FROM_SWAP        = 0x0800,  // If Image is on Net, copy and run from the swap file.
    IMAGE_FILE_SYSTEM                   = 0x1000,  // System File.
    IMAGE_FILE_DLL                      = 0x2000,  // File is a DLL.
    IMAGE_FILE_UP_SYSTEM_ONLY           = 0x4000,  // File should only be run on a UP machine
    IMAGE_FILE_BYTES_REVERSED_HI        = 0x8000,  // Bytes of machine word are reversed.
} characteristics_t;

const char * machine_to_string(machine_t machine);
const char * characteristics_to_string(characteristics_t value);

bool read_coff_file_header(FILE *infile, coff_file_header_t *coff_file_header);
void print_coff_file_header(const coff_file_header_t *coff_file_header);

#endif
