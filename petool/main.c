//#include <Windows.h>
//#include <winnt.h>
#include <stdint.h>

typedef struct {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
} coff_file_header_t;

typedef enum {
    IMAGE_FILE_MACHINE_UNKNOWN =          0,
    //IMAGE_FILE_MACHINE_ALPHA =            0x0184,  // Alpha_AXP
    //IMAGE_FILE_MACHINE_ALPHA64 =          0x0284,  // ALPHA64
    IMAGE_FILE_MACHINE_AM33 =             0x01d3,
    IMAGE_FILE_MACHINE_AMD64 =            0x8664,  // AMD64 (K8)
    IMAGE_FILE_MACHINE_ARM =              0x01c0,  // ARM Little-Endian
    IMAGE_FILE_MACHINE_ARMNT =            0x01c4,  // ARM Thumb-2 Little-Endian
    //IMAGE_FILE_MACHINE_AXP64 =            IMAGE_FILE_MACHINE_ALPHA64,
    IMAGE_FILE_MACHINE_ARM64 = 0xAA64,  // from spec
    //IMAGE_FILE_MACHINE_CEE =              0xC0EE,
    //IMAGE_FILE_MACHINE_CEF =              0x0CEF,
    IMAGE_FILE_MACHINE_EBC =              0x0EBC,  // EFI Byte Code
    IMAGE_FILE_MACHINE_I386 =             0x014c,  // Intel 386.
    IMAGE_FILE_MACHINE_IA64 =             0x0200,  // Intel 64
    IMAGE_FILE_MACHINE_M32R =             0x9041,  // M32R little-endian
    IMAGE_FILE_MACHINE_MIPS16 =           0x0266,  // MIPS
    IMAGE_FILE_MACHINE_MIPSFPU =          0x0366,  // MIPS
    IMAGE_FILE_MACHINE_MIPSFPU16 =        0x0466,  // MIPS
    IMAGE_FILE_MACHINE_POWERPC =          0x01F0,  // IBM PowerPC Little-Endian
    IMAGE_FILE_MACHINE_POWERPCFP =        0x01f1,
    //IMAGE_FILE_MACHINE_R10000 =           0x0168,  // MIPS little-endian
    //IMAGE_FILE_MACHINE_R3000 =            0x0162,  // MIPS little-endian, 0x160 big-endian
    IMAGE_FILE_MACHINE_R4000 =            0x0166,  // MIPS little-endian
    IMAGE_FILE_MACHINE_SH3 =              0x01a2,  // SH3 little-endian
    IMAGE_FILE_MACHINE_SH3DSP =           0x01a3,
    //IMAGE_FILE_MACHINE_SH3E =             0x01a4,  // SH3E little-endian
    IMAGE_FILE_MACHINE_SH4 =              0x01a6,  // SH4 little-endian
    IMAGE_FILE_MACHINE_SH5 =              0x01a8,  // SH5
    IMAGE_FILE_MACHINE_THUMB =            0x01c2,  // ARM Thumb/Thumb-2 Little-Endian
    //IMAGE_FILE_MACHINE_TRICORE =          0x0520,  // Infineon
    IMAGE_FILE_MACHINE_WCEMIPSV2 =        0x0169,  // MIPS little-endian WCE v2
} machine_t;

typedef enum {
    IMAGE_FILE_RELOCS_STRIPPED =          0x0001,  // Relocation info stripped from file.
    IMAGE_FILE_EXECUTABLE_IMAGE =         0x0002,  // File is executable  (i.e. no unresolved external references).
    IMAGE_FILE_LINE_NUMS_STRIPPED =       0x0004,  // Line nunbers stripped from file.
    IMAGE_FILE_LOCAL_SYMS_STRIPPED =      0x0008,  // Local symbols stripped from file.
    IMAGE_FILE_AGGRESIVE_WS_TRIM =        0x0010,  // Aggressively trim working set
    IMAGE_FILE_LARGE_ADDRESS_AWARE =      0x0020,  // App can handle >2gb addresses
    IMAGE_FILE_BYTES_REVERSED_LO =        0x0080,  // Bytes of machine word are reversed.
    IMAGE_FILE_32BIT_MACHINE =            0x0100,  // 32 bit word machine.
    IMAGE_FILE_DEBUG_STRIPPED =           0x0200,  // Debugging info stripped from file in .DBG file
    IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP =  0x0400,  // If Image is on removable media, copy and run from the swap file.
    IMAGE_FILE_NET_RUN_FROM_SWAP =        0x0800,  // If Image is on Net, copy and run from the swap file.
    IMAGE_FILE_SYSTEM =                   0x1000,  // System File.
    IMAGE_FILE_DLL =                      0x2000,  // File is a DLL.
    IMAGE_FILE_UP_SYSTEM_ONLY =           0x4000,  // File should only be run on a UP machine
    IMAGE_FILE_BYTES_REVERSED_HI =        0x8000,  // Bytes of machine word are reversed.
} characteristics_t;

typedef struct {
    uint32_t VirtualAddress;
    uint32_t Size;
} data_directory_t;


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

typedef enum {
    IMAGE_SUBSYSTEM_UNKNOWN =             0,   // Unknown subsystem.
    IMAGE_SUBSYSTEM_NATIVE =              1,   // Image doesn't require a subsystem.
    IMAGE_SUBSYSTEM_WINDOWS_GUI =         2,   // Image runs in the Windows GUI subsystem.
    IMAGE_SUBSYSTEM_WINDOWS_CUI =         3,   // Image runs in the Windows character subsystem.
    //IMAGE_SUBSYSTEM_OS2_CUI =             5,   // image runs in the OS/2 character subsystem.
    IMAGE_SUBSYSTEM_POSIX_CUI =           7,   // image runs in the Posix character subsystem.
    //IMAGE_SUBSYSTEM_NATIVE_WINDOWS =      8,   // image is a native Win9x driver.
    IMAGE_SUBSYSTEM_WINDOWS_CE_GUI =      9,   // Image runs in the Windows CE subsystem.
    IMAGE_SUBSYSTEM_EFI_APPLICATION =     10,  //
    IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11,   //
    IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER =  12,  //
    IMAGE_SUBSYSTEM_EFI_ROM =             13,
    IMAGE_SUBSYSTEM_XBOX =                14,
    //IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION = 16,
} subsystem_t;

typedef enum {
    //IMAGE_LIBRARY_PROCESS_INIT =           0x0001,     // Reserved.
    //IMAGE_LIBRARY_PROCESS_TERM =           0x0002,     // Reserved.
    //IMAGE_LIBRARY_THREAD_INIT =            0x0004,     // Reserved.
    //IMAGE_LIBRARY_THREAD_TERM =            0x0008,     // Reserved.

    //IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA =   0x0020,  // Image can handle a high entropy 64-bit virtual address space.
    IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE = 0x0040,     // DLL can move.
    IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY =   0x0080,     // Code Integrity Image
    IMAGE_DLLCHARACTERISTICS_NX_COMPAT =   0x0100,     // Image is NX compatible
    IMAGE_DLLCHARACTERISTICS_NO_ISOLATION = 0x0200,     // Image understands isolation and doesn't want it
    IMAGE_DLLCHARACTERISTICS_NO_SEH =      0x0400,     // Image does not use SEH.  No SE handler may reside in this image
    IMAGE_DLLCHARACTERISTICS_NO_BIND =     0x0800,     // Do not bind this image.
    //IMAGE_DLLCHARACTERISTICS_APPCONTAINER = 0x1000,     // Image should execute in an AppContainer
    IMAGE_DLLCHARACTERISTICS_WDM_DRIVER =  0x2000,     // Driver uses WDM model
    //IMAGE_DLLCHARACTERISTICS_GUARD_CF =    0x4000,     // Image supports Control Flow Guard.
    IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE =    0x8000,
} dll_characteristics_t;

typedef enum {
    DATA_DIR_EXPORT_TABLE = 0,
    DATA_DIR_IMPORT_TABLE,
    DATA_DIR_RESOURCE_TABLE,
    DATA_DIR_EXCEPTION_TABLE,
    DATA_DIR_CERTIFICATE_TABLE,
    DATA_DIR_BASE_RELOCATION_TABLE,
    DATA_DIR_DEBUG,
    DATA_DIR_ARCHITECTURE,
    DATA_DIR_GLOBAL_PTR,
    DATA_DIR_TLS_TABLE,
    DATA_DIR_LOAD_CONFIG_TABLE,
    DATA_DIR_BOUND_IMPORT,
    DATA_DIR_IAT,
    DATA_DIR_DELAY_IMPORT_DESCRIPTOR,
    DATA_DIR_CLR_RUNTIME_HEADER,
    DATA_DIR_RESERVED,
} data_directory_index_t;

typedef struct {
    uint8_t Name[8];

    //union {
    //        DWORD   PhysicalAddress;
    //        DWORD   VirtualSize;
    //} Misc;
    uint32_t VirtualSize;

    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
} section_header_t;


//
// Section characteristics.
//
typedef enum {
    //      IMAGE_SCN_TYPE_REG                   0x00000000  // Reserved.
    //      IMAGE_SCN_TYPE_DSECT                 0x00000001  // Reserved.
    //      IMAGE_SCN_TYPE_NOLOAD                0x00000002  // Reserved.
    //      IMAGE_SCN_TYPE_GROUP                 0x00000004  // Reserved.
    IMAGE_SCN_TYPE_NO_PAD =               0x00000008,  // Reserved.
    //      IMAGE_SCN_TYPE_COPY                  0x00000010  // Reserved.

    IMAGE_SCN_CNT_CODE =                  0x00000020,  // Section contains code.
    IMAGE_SCN_CNT_INITIALIZED_DATA =      0x00000040,  // Section contains initialized data.
    IMAGE_SCN_CNT_UNINITIALIZED_DATA =    0x00000080,  // Section contains uninitialized data.

    IMAGE_SCN_LNK_OTHER =                 0x00000100,  // Reserved.
    IMAGE_SCN_LNK_INFO =                  0x00000200,  // Section contains comments or some other type of information.
    //      IMAGE_SCN_TYPE_OVER                  0x00000400  // Reserved.
    IMAGE_SCN_LNK_REMOVE =                0x00000800,  // Section contents will not become part of image.
    IMAGE_SCN_LNK_COMDAT =                0x00001000,  // Section contents comdat.
    //                                           0x00002000  // Reserved.
    //      IMAGE_SCN_MEM_PROTECTED - Obsolete   0x00004000
    //IMAGE_SCN_NO_DEFER_SPEC_EXC =         0x00004000,  // Reset speculative exceptions handling bits in the TLB entries for this section.
    IMAGE_SCN_GPREL =                     0x00008000,  // Section content can be accessed relative to GP
    //IMAGE_SCN_MEM_FARDATA =               0x00008000,
    //      IMAGE_SCN_MEM_SYSHEAP  - Obsolete    0x00010000
    IMAGE_SCN_MEM_PURGEABLE =             0x00020000,
    IMAGE_SCN_MEM_16BIT =                 0x00020000,
    IMAGE_SCN_MEM_LOCKED =                0x00040000,
    IMAGE_SCN_MEM_PRELOAD =               0x00080000,

    IMAGE_SCN_ALIGN_1BYTES =              0x00100000,  //
    IMAGE_SCN_ALIGN_2BYTES =              0x00200000,  //
    IMAGE_SCN_ALIGN_4BYTES =              0x00300000,  //
    IMAGE_SCN_ALIGN_8BYTES =              0x00400000,  //
    IMAGE_SCN_ALIGN_16BYTES =             0x00500000,  // Default alignment if no others are specified.
    IMAGE_SCN_ALIGN_32BYTES =             0x00600000,  //
    IMAGE_SCN_ALIGN_64BYTES =             0x00700000,  //
    IMAGE_SCN_ALIGN_128BYTES =            0x00800000,  //
    IMAGE_SCN_ALIGN_256BYTES =            0x00900000,  //
    IMAGE_SCN_ALIGN_512BYTES =            0x00A00000,  //
    IMAGE_SCN_ALIGN_1024BYTES =           0x00B00000,  //
    IMAGE_SCN_ALIGN_2048BYTES =           0x00C00000,  //
    IMAGE_SCN_ALIGN_4096BYTES =           0x00D00000,  //
    IMAGE_SCN_ALIGN_8192BYTES =           0x00E00000,  //
    // Unused                                    0x00F00000
    //IMAGE_SCN_ALIGN_MASK =                0x00F00000,

    IMAGE_SCN_LNK_NRELOC_OVFL =           0x01000000,  // Section contains extended relocations.
    IMAGE_SCN_MEM_DISCARDABLE =           0x02000000,  // Section can be discarded.
    IMAGE_SCN_MEM_NOT_CACHED =            0x04000000,  // Section is not cachable.
    IMAGE_SCN_MEM_NOT_PAGED =             0x08000000,  // Section is not pageable.
    IMAGE_SCN_MEM_SHARED =                0x10000000,  // Section is shareable.
    IMAGE_SCN_MEM_EXECUTE =               0x20000000,  // Section is executable.
    IMAGE_SCN_MEM_READ =                  0x40000000,  // Section is readable.
    IMAGE_SCN_MEM_WRITE =                 0x80000000,  // Section is writeable.
} section_flags_t;

typedef struct {
    //union {
    //    DWORD   VirtualAddress;
    //    DWORD   RelocCount;             // Set to the real count when IMAGE_SCN_LNK_NRELOC_OVFL is set
    //} DUMMYUNIONNAME;
    uint32_t VirtualAddress;

    uint32_t SymbolTableIndex;
    uint16_t Type;
} coff_relocation_t;

typedef enum {
//
// I386 relocation types.
//
IMAGE_REL_I386_ABSOLUTE =         0x0000, // Reference is absolute, no relocation is necessary
IMAGE_REL_I386_DIR16 =            0x0001, // Direct 16-bit reference to the symbols virtual address
IMAGE_REL_I386_REL16 =            0x0002, // PC-relative 16-bit reference to the symbols virtual address
IMAGE_REL_I386_DIR32 =            0x0006, // Direct 32-bit reference to the symbols virtual address
IMAGE_REL_I386_DIR32NB =          0x0007, // Direct 32-bit reference to the symbols virtual address, base not included
IMAGE_REL_I386_SEG12 =            0x0009, // Direct 16-bit reference to the segment-selector bits of a 32-bit virtual address
IMAGE_REL_I386_SECTION =          0x000A,
IMAGE_REL_I386_SECREL =           0x000B,
IMAGE_REL_I386_TOKEN =            0x000C, // clr token
IMAGE_REL_I386_SECREL7 =          0x000D, // 7 bit offset from base of section containing target
IMAGE_REL_I386_REL32 =            0x0014, // PC-relative 32-bit reference to the symbols virtual address

//
// MIPS relocation types.
//
IMAGE_REL_MIPS_ABSOLUTE =         0x0000, // Reference is absolute, no relocation is necessary
IMAGE_REL_MIPS_REFHALF =          0x0001,
IMAGE_REL_MIPS_REFWORD =          0x0002,
IMAGE_REL_MIPS_JMPADDR =          0x0003,
IMAGE_REL_MIPS_REFHI =            0x0004,
IMAGE_REL_MIPS_REFLO =            0x0005,
IMAGE_REL_MIPS_GPREL =            0x0006,
IMAGE_REL_MIPS_LITERAL =          0x0007,
IMAGE_REL_MIPS_SECTION =          0x000A,
IMAGE_REL_MIPS_SECREL =           0x000B,
IMAGE_REL_MIPS_SECRELLO =         0x000C, // Low 16-bit section relative referemce (used for >32k TLS)
IMAGE_REL_MIPS_SECRELHI =         0x000D, // High 16-bit section relative reference (used for >32k TLS)
IMAGE_REL_MIPS_TOKEN =            0x000E, // clr token
IMAGE_REL_MIPS_JMPADDR16 =        0x0010,
IMAGE_REL_MIPS_REFWORDNB =        0x0022,
IMAGE_REL_MIPS_PAIR =             0x0025,

//
// Alpha Relocation types.
//
IMAGE_REL_ALPHA_ABSOLUTE =        0x0000,
IMAGE_REL_ALPHA_REFLONG =         0x0001,
IMAGE_REL_ALPHA_REFQUAD =         0x0002,
IMAGE_REL_ALPHA_GPREL32 =         0x0003,
IMAGE_REL_ALPHA_LITERAL =         0x0004,
IMAGE_REL_ALPHA_LITUSE =          0x0005,
IMAGE_REL_ALPHA_GPDISP =          0x0006,
IMAGE_REL_ALPHA_BRADDR =          0x0007,
IMAGE_REL_ALPHA_HINT =            0x0008,
IMAGE_REL_ALPHA_INLINE_REFLONG =  0x0009,
IMAGE_REL_ALPHA_REFHI =           0x000A,
IMAGE_REL_ALPHA_REFLO =           0x000B,
IMAGE_REL_ALPHA_PAIR =            0x000C,
IMAGE_REL_ALPHA_MATCH =           0x000D,
IMAGE_REL_ALPHA_SECTION =         0x000E,
IMAGE_REL_ALPHA_SECREL =          0x000F,
IMAGE_REL_ALPHA_REFLONGNB =       0x0010,
IMAGE_REL_ALPHA_SECRELLO =        0x0011, // Low 16-bit section relative reference
IMAGE_REL_ALPHA_SECRELHI =        0x0012, // High 16-bit section relative reference
IMAGE_REL_ALPHA_REFQ3 =           0x0013, // High 16 bits of 48 bit reference
IMAGE_REL_ALPHA_REFQ2 =           0x0014, // Middle 16 bits of 48 bit reference
IMAGE_REL_ALPHA_REFQ1 =           0x0015, // Low 16 bits of 48 bit reference
IMAGE_REL_ALPHA_GPRELLO =         0x0016, // Low 16-bit GP relative reference
IMAGE_REL_ALPHA_GPRELHI =         0x0017, // High 16-bit GP relative reference

//
// IBM PowerPC relocation types.
//
IMAGE_REL_PPC_ABSOLUTE =          0x0000, // NOP
IMAGE_REL_PPC_ADDR64 =            0x0001, // 64-bit address
IMAGE_REL_PPC_ADDR32 =            0x0002, // 32-bit address
IMAGE_REL_PPC_ADDR24 =            0x0003, // 26-bit address, shifted left 2 (branch absolute)
IMAGE_REL_PPC_ADDR16 =            0x0004, // 16-bit address
IMAGE_REL_PPC_ADDR14 =            0x0005, // 16-bit address, shifted left 2 (load doubleword)
IMAGE_REL_PPC_REL24 =             0x0006, // 26-bit PC-relative offset, shifted left 2 (branch relative)
IMAGE_REL_PPC_REL14 =             0x0007, // 16-bit PC-relative offset, shifted left 2 (br cond relative)
IMAGE_REL_PPC_TOCREL16 =          0x0008, // 16-bit offset from TOC base
IMAGE_REL_PPC_TOCREL14 =          0x0009, // 16-bit offset from TOC base, shifted left 2 (load doubleword)

IMAGE_REL_PPC_ADDR32NB =          0x000A, // 32-bit addr w/o image base
IMAGE_REL_PPC_SECREL =            0x000B, // va of containing section (as in an image sectionhdr)
IMAGE_REL_PPC_SECTION =           0x000C, // sectionheader number
IMAGE_REL_PPC_IFGLUE =            0x000D, // substitute TOC restore instruction iff symbol is glue code
IMAGE_REL_PPC_IMGLUE =            0x000E, // symbol is glue code; virtual address is TOC restore instruction
IMAGE_REL_PPC_SECREL16 =          0x000F, // va of containing section (limited to 16 bits)
IMAGE_REL_PPC_REFHI =             0x0010,
IMAGE_REL_PPC_REFLO =             0x0011,
IMAGE_REL_PPC_PAIR =              0x0012,
IMAGE_REL_PPC_SECRELLO =          0x0013, // Low 16-bit section relative reference (used for >32k TLS)
IMAGE_REL_PPC_SECRELHI =          0x0014, // High 16-bit section relative reference (used for >32k TLS)
IMAGE_REL_PPC_GPREL =             0x0015,
IMAGE_REL_PPC_TOKEN =             0x0016, // clr token

IMAGE_REL_PPC_TYPEMASK =          0x00FF, // mask to isolate above values in IMAGE_RELOCATION.Type

// Flag bits in IMAGE_RELOCATION.TYPE

IMAGE_REL_PPC_NEG =               0x0100, // subtract reloc value rather than adding it
IMAGE_REL_PPC_BRTAKEN =           0x0200, // fix branch prediction bit to predict branch taken
IMAGE_REL_PPC_BRNTAKEN =          0x0400, // fix branch prediction bit to predict branch not taken
IMAGE_REL_PPC_TOCDEFN =           0x0800, // toc slot defined in file (or, data in toc)

//
// Hitachi SH3 relocation types.
//
IMAGE_REL_SH3_ABSOLUTE =          0x0000, // No relocation
IMAGE_REL_SH3_DIRECT16 =          0x0001, // 16 bit direct
IMAGE_REL_SH3_DIRECT32 =          0x0002, // 32 bit direct
IMAGE_REL_SH3_DIRECT8 =           0x0003, // 8 bit direct, -128..255
IMAGE_REL_SH3_DIRECT8_WORD =      0x0004, // 8 bit direct .W (0 ext.)
IMAGE_REL_SH3_DIRECT8_LONG =      0x0005, // 8 bit direct .L (0 ext.)
IMAGE_REL_SH3_DIRECT4 =           0x0006, // 4 bit direct (0 ext.)
IMAGE_REL_SH3_DIRECT4_WORD =      0x0007, // 4 bit direct .W (0 ext.)
IMAGE_REL_SH3_DIRECT4_LONG =      0x0008, // 4 bit direct .L (0 ext.)
IMAGE_REL_SH3_PCREL8_WORD =       0x0009, // 8 bit PC relative .W
IMAGE_REL_SH3_PCREL8_LONG =       0x000A, // 8 bit PC relative .L
IMAGE_REL_SH3_PCREL12_WORD =      0x000B, // 12 LSB PC relative .W
IMAGE_REL_SH3_STARTOF_SECTION =   0x000C, // Start of EXE section
IMAGE_REL_SH3_SIZEOF_SECTION =    0x000D, // Size of EXE section
IMAGE_REL_SH3_SECTION =           0x000E, // Section table index
IMAGE_REL_SH3_SECREL =            0x000F, // Offset within section
IMAGE_REL_SH3_DIRECT32_NB =       0x0010, // 32 bit direct not based
IMAGE_REL_SH3_GPREL4_LONG =       0x0011, // GP-relative addressing
IMAGE_REL_SH3_TOKEN =             0x0012, // clr token
IMAGE_REL_SHM_PCRELPT =           0x0013, // Offset from current
                                                //  instruction in longwords
                                                //  if not NOMODE, insert the
                                                //  inverse of the low bit at
                                                //  bit 32 to select PTA/PTB
IMAGE_REL_SHM_REFLO =             0x0014, // Low bits of 32-bit address
IMAGE_REL_SHM_REFHALF =           0x0015, // High bits of 32-bit address
IMAGE_REL_SHM_RELLO =             0x0016, // Low bits of relative reference
IMAGE_REL_SHM_RELHALF =           0x0017, // High bits of relative reference
IMAGE_REL_SHM_PAIR =              0x0018, // offset operand for relocation

IMAGE_REL_SH_NOMODE =             0x8000, // relocation ignores section mode


IMAGE_REL_ARM_ABSOLUTE =          0x0000, // No relocation required
IMAGE_REL_ARM_ADDR32 =            0x0001, // 32 bit address
IMAGE_REL_ARM_ADDR32NB =          0x0002, // 32 bit address w/o image base
IMAGE_REL_ARM_BRANCH24 =          0x0003, // 24 bit offset << 2 & sign ext.
IMAGE_REL_ARM_BRANCH11 =          0x0004, // Thumb: 2 11 bit offsets
IMAGE_REL_ARM_TOKEN =             0x0005, // clr token
IMAGE_REL_ARM_GPREL12 =           0x0006, // GP-relative addressing (ARM)
IMAGE_REL_ARM_GPREL7 =            0x0007, // GP-relative addressing (Thumb)
IMAGE_REL_ARM_BLX24 =             0x0008,
IMAGE_REL_ARM_BLX11 =             0x0009,
IMAGE_REL_ARM_SECTION =           0x000E, // Section table index
IMAGE_REL_ARM_SECREL =            0x000F, // Offset within section
IMAGE_REL_ARM_MOV32A =            0x0010, // ARM: MOVW/MOVT
IMAGE_REL_ARM_MOV32 =             0x0010, // ARM: MOVW/MOVT (deprecated)
IMAGE_REL_ARM_MOV32T =            0x0011, // Thumb: MOVW/MOVT
IMAGE_REL_THUMB_MOV32 =           0x0011, // Thumb: MOVW/MOVT (deprecated)
IMAGE_REL_ARM_BRANCH20T =         0x0012, // Thumb: 32-bit conditional B
IMAGE_REL_THUMB_BRANCH20 =        0x0012, // Thumb: 32-bit conditional B (deprecated)
IMAGE_REL_ARM_BRANCH24T =         0x0014, // Thumb: 32-bit B or BL
IMAGE_REL_THUMB_BRANCH24 =        0x0014, // Thumb: 32-bit B or BL (deprecated)
IMAGE_REL_ARM_BLX23T =            0x0015, // Thumb: BLX immediate
IMAGE_REL_THUMB_BLX23 =           0x0015, // Thumb: BLX immediate (deprecated)

IMAGE_REL_AM_ABSOLUTE =           0x0000,
IMAGE_REL_AM_ADDR32 =             0x0001,
IMAGE_REL_AM_ADDR32NB =           0x0002,
IMAGE_REL_AM_CALL32 =             0x0003,
IMAGE_REL_AM_FUNCINFO =           0x0004,
IMAGE_REL_AM_REL32_1 =            0x0005,
IMAGE_REL_AM_REL32_2 =            0x0006,
IMAGE_REL_AM_SECREL =             0x0007,
IMAGE_REL_AM_SECTION =            0x0008,
IMAGE_REL_AM_TOKEN =              0x0009,

//
// x64 relocations
//
IMAGE_REL_AMD64_ABSOLUTE =        0x0000, // Reference is absolute, no relocation is necessary
IMAGE_REL_AMD64_ADDR64 =          0x0001, // 64-bit address (VA).
IMAGE_REL_AMD64_ADDR32 =          0x0002, // 32-bit address (VA).
IMAGE_REL_AMD64_ADDR32NB =        0x0003, // 32-bit address w/o image base (RVA).
IMAGE_REL_AMD64_REL32 =           0x0004, // 32-bit relative address from byte following reloc
IMAGE_REL_AMD64_REL32_1 =         0x0005, // 32-bit relative address from byte distance 1 from reloc
IMAGE_REL_AMD64_REL32_2 =         0x0006, // 32-bit relative address from byte distance 2 from reloc
IMAGE_REL_AMD64_REL32_3 =         0x0007, // 32-bit relative address from byte distance 3 from reloc
IMAGE_REL_AMD64_REL32_4 =         0x0008, // 32-bit relative address from byte distance 4 from reloc
IMAGE_REL_AMD64_REL32_5 =         0x0009, // 32-bit relative address from byte distance 5 from reloc
IMAGE_REL_AMD64_SECTION =         0x000A, // Section index
IMAGE_REL_AMD64_SECREL =          0x000B, // 32 bit offset from base of section containing target
IMAGE_REL_AMD64_SECREL7 =         0x000C, // 7 bit unsigned offset from base of section containing target
IMAGE_REL_AMD64_TOKEN =           0x000D, // 32 bit metadata token
IMAGE_REL_AMD64_SREL32 =          0x000E, // 32 bit signed span-dependent value emitted into object
IMAGE_REL_AMD64_PAIR =            0x000F,
IMAGE_REL_AMD64_SSPAN32 =         0x0010, // 32 bit signed span-dependent value applied at link time

//
// IA64 relocation types.
//
IMAGE_REL_IA64_ABSOLUTE =         0x0000,
IMAGE_REL_IA64_IMM14 =            0x0001,
IMAGE_REL_IA64_IMM22 =            0x0002,
IMAGE_REL_IA64_IMM64 =            0x0003,
IMAGE_REL_IA64_DIR32 =            0x0004,
IMAGE_REL_IA64_DIR64 =            0x0005,
IMAGE_REL_IA64_PCREL21B =         0x0006,
IMAGE_REL_IA64_PCREL21M =         0x0007,
IMAGE_REL_IA64_PCREL21F =         0x0008,
IMAGE_REL_IA64_GPREL22 =          0x0009,
IMAGE_REL_IA64_LTOFF22 =          0x000A,
IMAGE_REL_IA64_SECTION =          0x000B,
IMAGE_REL_IA64_SECREL22 =         0x000C,
IMAGE_REL_IA64_SECREL64I =        0x000D,
IMAGE_REL_IA64_SECREL32 =         0x000E,
//
IMAGE_REL_IA64_DIR32NB =          0x0010,
IMAGE_REL_IA64_SREL14 =           0x0011,
IMAGE_REL_IA64_SREL22 =           0x0012,
IMAGE_REL_IA64_SREL32 =           0x0013,
IMAGE_REL_IA64_UREL32 =           0x0014,
IMAGE_REL_IA64_PCREL60X =         0x0015, // This is always a BRL and never converted
IMAGE_REL_IA64_PCREL60B =         0x0016, // If possible, convert to MBB bundle with NOP.B in slot 1
IMAGE_REL_IA64_PCREL60F =         0x0017, // If possible, convert to MFB bundle with NOP.F in slot 1
IMAGE_REL_IA64_PCREL60I =         0x0018, // If possible, convert to MIB bundle with NOP.I in slot 1
IMAGE_REL_IA64_PCREL60M =         0x0019, // If possible, convert to MMB bundle with NOP.M in slot 1
IMAGE_REL_IA64_IMMGPREL64 =       0x001A,
IMAGE_REL_IA64_TOKEN =            0x001B, // clr token
IMAGE_REL_IA64_GPREL32 =          0x001C,
IMAGE_REL_IA64_ADDEND =           0x001F,

//
// CEF relocation types.
//
IMAGE_REL_CEF_ABSOLUTE =          0x0000, // Reference is absolute, no relocation is necessary
IMAGE_REL_CEF_ADDR32 =            0x0001, // 32-bit address (VA).
IMAGE_REL_CEF_ADDR64 =            0x0002, // 64-bit address (VA).
IMAGE_REL_CEF_ADDR32NB =          0x0003, // 32-bit address w/o image base (RVA).
IMAGE_REL_CEF_SECTION =           0x0004, // Section index
IMAGE_REL_CEF_SECREL =            0x0005, // 32 bit offset from base of section containing target
IMAGE_REL_CEF_TOKEN =             0x0006, // 32 bit metadata token

//
// clr relocation types.
//
IMAGE_REL_CEE_ABSOLUTE =          0x0000, // Reference is absolute, no relocation is necessary
IMAGE_REL_CEE_ADDR32 =            0x0001, // 32-bit address (VA).
IMAGE_REL_CEE_ADDR64 =            0x0002, // 64-bit address (VA).
IMAGE_REL_CEE_ADDR32NB =          0x0003, // 32-bit address w/o image base (RVA).
IMAGE_REL_CEE_SECTION =           0x0004, // Section index
IMAGE_REL_CEE_SECREL =            0x0005, // 32 bit offset from base of section containing target
IMAGE_REL_CEE_TOKEN =             0x0006, // 32 bit metadata token


IMAGE_REL_M32R_ABSOLUTE =         0x0000, // No relocation required
IMAGE_REL_M32R_ADDR32 =           0x0001, // 32 bit address
IMAGE_REL_M32R_ADDR32NB =         0x0002, // 32 bit address w/o image base
IMAGE_REL_M32R_ADDR24 =           0x0003, // 24 bit address
IMAGE_REL_M32R_GPREL16 =          0x0004, // GP relative addressing
IMAGE_REL_M32R_PCREL24 =          0x0005, // 24 bit offset << 2 & sign ext.
IMAGE_REL_M32R_PCREL16 =          0x0006, // 16 bit offset << 2 & sign ext.
IMAGE_REL_M32R_PCREL8 =           0x0007, // 8 bit offset << 2 & sign ext.
IMAGE_REL_M32R_REFHALF =          0x0008, // 16 MSBs
IMAGE_REL_M32R_REFHI =            0x0009, // 16 MSBs; adj for LSB sign ext.
IMAGE_REL_M32R_REFLO =            0x000A, // 16 LSBs
IMAGE_REL_M32R_PAIR =             0x000B, // Link HI and LO
IMAGE_REL_M32R_SECTION =          0x000C, // Section table index
IMAGE_REL_M32R_SECREL32 =         0x000D, // 32 bit section relative reference
IMAGE_REL_M32R_TOKEN =            0x000E, // clr token

IMAGE_REL_EBC_ABSOLUTE =          0x0000, // No relocation required
IMAGE_REL_EBC_ADDR32NB =          0x0001, // 32 bit address w/o image base
IMAGE_REL_EBC_REL32 =             0x0002, // 32-bit relative address from byte following reloc
IMAGE_REL_EBC_SECTION =           0x0003, // Section table index
IMAGE_REL_EBC_SECREL =            0x0004, // Offset within section
} coff_relocation_type_t;

typedef struct {
    union {
        uint32_t SymbolTableIndex;               // Symbol table index of function name if Linenumber is 0.
        uint32_t VirtualAddress;                 // Virtual address of line number.
    } Type;
    uint16_t Linenumber;                         // Line number.
} coff_line_number_t;

typedef struct {
    union {
        uint8_t ShortName[8];
        struct {
            uint32_t   Short;     // if 0, use LongName
            uint32_t   Long;      // offset into string table
        } Name;
        uint32_t   LongName[2];    // PBYTE [2]
    } Name;
    uint32_t   Value;
    uint16_t   SectionNumber;
    uint16_t    Type;
    uint8_t    StorageClass;
    uint8_t   NumberOfAuxSymbols;
} symbol_table_record_t;

// Section Number Values
#define IMAGE_SYM_UNDEFINED           (SHORT)0          // Symbol is undefined or is common.
#define IMAGE_SYM_ABSOLUTE            (SHORT)-1         // Symbol is an absolute value.
#define IMAGE_SYM_DEBUG               (SHORT)-2         // Symbol is a special debug item.

//
// Section Types
// Type (fundamental) values.
#define IMAGE_SYM_TYPE_NULL                 0x0000  // no type.
#define IMAGE_SYM_TYPE_VOID                 0x0001  //
#define IMAGE_SYM_TYPE_CHAR                 0x0002  // type character.
#define IMAGE_SYM_TYPE_SHORT                0x0003  // type short integer.
#define IMAGE_SYM_TYPE_INT                  0x0004  //
#define IMAGE_SYM_TYPE_LONG                 0x0005  //
#define IMAGE_SYM_TYPE_FLOAT                0x0006  //
#define IMAGE_SYM_TYPE_DOUBLE               0x0007  //
#define IMAGE_SYM_TYPE_STRUCT               0x0008  //
#define IMAGE_SYM_TYPE_UNION                0x0009  //
#define IMAGE_SYM_TYPE_ENUM                 0x000A  // enumeration.
#define IMAGE_SYM_TYPE_MOE                  0x000B  // member of enumeration.
#define IMAGE_SYM_TYPE_BYTE                 0x000C  //
#define IMAGE_SYM_TYPE_WORD                 0x000D  //
#define IMAGE_SYM_TYPE_UINT                 0x000E  //
#define IMAGE_SYM_TYPE_DWORD                0x000F  //
#define IMAGE_SYM_TYPE_PCODE                0x8000  //

//
// Type (derived) values.
//
#define IMAGE_SYM_DTYPE_NULL                0       // no derived type.
#define IMAGE_SYM_DTYPE_POINTER             1       // pointer.
#define IMAGE_SYM_DTYPE_FUNCTION            2       // function.
#define IMAGE_SYM_DTYPE_ARRAY               3       // array.



//
// Symbol Table Storage Classes
// Storage classes.
//
#define IMAGE_SYM_CLASS_END_OF_FUNCTION     (BYTE )-1
#define IMAGE_SYM_CLASS_NULL                0x0000
#define IMAGE_SYM_CLASS_AUTOMATIC           0x0001
#define IMAGE_SYM_CLASS_EXTERNAL            0x0002
#define IMAGE_SYM_CLASS_STATIC              0x0003
#define IMAGE_SYM_CLASS_REGISTER            0x0004
#define IMAGE_SYM_CLASS_EXTERNAL_DEF        0x0005
#define IMAGE_SYM_CLASS_LABEL               0x0006
#define IMAGE_SYM_CLASS_UNDEFINED_LABEL     0x0007
#define IMAGE_SYM_CLASS_MEMBER_OF_STRUCT    0x0008
#define IMAGE_SYM_CLASS_ARGUMENT            0x0009
#define IMAGE_SYM_CLASS_STRUCT_TAG          0x000A
#define IMAGE_SYM_CLASS_MEMBER_OF_UNION     0x000B
#define IMAGE_SYM_CLASS_UNION_TAG           0x000C
#define IMAGE_SYM_CLASS_TYPE_DEFINITION     0x000D
#define IMAGE_SYM_CLASS_UNDEFINED_STATIC    0x000E
#define IMAGE_SYM_CLASS_ENUM_TAG            0x000F
#define IMAGE_SYM_CLASS_MEMBER_OF_ENUM      0x0010
#define IMAGE_SYM_CLASS_REGISTER_PARAM      0x0011
#define IMAGE_SYM_CLASS_BIT_FIELD           0x0012

#define IMAGE_SYM_CLASS_FAR_EXTERNAL        0x0044  //

#define IMAGE_SYM_CLASS_BLOCK               0x0064
#define IMAGE_SYM_CLASS_FUNCTION            0x0065
#define IMAGE_SYM_CLASS_END_OF_STRUCT       0x0066
#define IMAGE_SYM_CLASS_FILE                0x0067
// new
#define IMAGE_SYM_CLASS_SECTION             0x0068
#define IMAGE_SYM_CLASS_WEAK_EXTERNAL       0x0069

#define IMAGE_SYM_CLASS_CLR_TOKEN           0x006B

/*
typedef union {
    struct {
        uint32_t TagIndex;                      // struct, union, or enum tag index
        union {
            struct {
                uint16_t Linenumber;             // declaration line number
                uint16_t    Size;                   // size of struct, union, or enum
            } LnSz;
           uint32_t    TotalSize;
        } Misc;
        union {
            struct {                            // if ISFCN, tag, or .bb
                uint32_t    PointerToLinenumber;
                uint32_t    PointerToNextFunction;
            } Function;
            struct {                            // if ISARY, up to 4 dimen.
                uint16_t     Dimension[4];
            } Array;
        } FcnAry;
        uint16_t    TvIndex;                        // tv index
    } Sym;

    struct {
        uint8_t    Name[IMAGE_SIZEOF_SYMBOL];
    } File;

    struct {
        uint32_t   Length;                         // section length
        uint16_t    NumberOfRelocations;            // number of relocation entries
        uint16_t    NumberOfLinenumbers;            // number of line numbers
        uint32_t   CheckSum;                       // checksum for communal
        uint16_t   Number;                         // section number to associate with
        uint8_t    Selection;                      // communal selection type
	uint8_t    bReserved;
	uint16_t   HighNumber;                     // high bits of the section number
    } Section;
    IMAGE_AUX_SYMBOL_TOKEN_DEF TokenDef;

    struct {
        uint32_t crc;
        uint8_t  rgbReserved[14];
    } CRC;
} aux_symbol_record_t;
*/

// Auxiliary Symbol Table record, always 18 bytes
typedef union {
    // Auxiliary Format 1: Function Definitions
    struct {
        uint32_t TagIndex;
        uint32_t TotalSize;
        uint32_t PointerToLinenumber;
        uint32_t PointerToNextFunction;
        uint16_t Unused;
    } FuncDef;

    // Auxiliary Format 2: .bf and .ef Symbols
    struct {
        uint32_t Unused_1;
        uint16_t Linenumber;
        uint16_t Unused_2;
        uint32_t Unused_3;
        uint32_t PointerToNextFunction;
        uint16_t Unused_4;
    } BfEf;

    // Auxiliary Format 3: Weak Externals
    struct {
        uint32_t TagIndex;
        uint32_t Characteristics;
        uint32_t Unused_1;
        uint32_t Unused_2;
        uint16_t Unused_3;
    } WeakExternals;

    // Auxiliary Format 4: Files
    uint8_t FileName[18];

    // Auxiliary Format 5: Section Definitions
    struct {
        uint32_t Length;
        uint16_t NumberOfRelocations;
        uint16_t NumberOfLinenumbers;
        uint32_t CheckSum;
        uint16_t Number;
        uint8_t  Selection;
        uint8_t  Unused[3];
    } SectionDef;
} aux_symbol_record_t;


//
// Aux Format 5 (Section Definitions) Selection field
// Communal selection types.
//
#define IMAGE_COMDAT_SELECT_NODUPLICATES    1
#define IMAGE_COMDAT_SELECT_ANY             2
#define IMAGE_COMDAT_SELECT_SAME_SIZE       3
#define IMAGE_COMDAT_SELECT_EXACT_MATCH     4
#define IMAGE_COMDAT_SELECT_ASSOCIATIVE     5
#define IMAGE_COMDAT_SELECT_LARGEST         6
#define IMAGE_COMDAT_SELECT_NEWEST          7

// CLR Toke Definition (Auxiliary Symbol token)
typedef struct {
    uint8_t  bAuxType;                  // IMAGE_AUX_SYMBOL_TYPE
    uint8_t  bReserved;                 // Must be 0
    uint32_t SymbolTableIndex;
    uint8_t  rgbReserved[12];           // Must be 0
} aux_symbol_token_t;

typedef struct {
    uint32_t dwLength;
    uint16_t wRevision;
    uint16_t wCertificateType;
    uint8_t  bCertificate[];
} attribute_certificate_record_t;

#define WIN_CERT_REVISION_1_0 0x0100
#define WIN_CERT_REVISION_2_0 0x0200
#define WIN_CERT_TYPE_X509 0x0001
#define WIN_CERT_TYPE_PKCS_SIGNED_DATA 0x0002
#define WIN_CERT_TYPE_RESERVED_1 0x0003
#define WIN_CERT_TYPE_TS_STACK_SIGNED 0x0004


typedef struct {
    //union {
    //    uint32_t AllAttributes;
    //    struct {
    //        uint32_t RvaBased : 1;             // Delay load version 2
    //        uint32_t ReservedAttributes : 31;
    //    };
    //} Attributes;
    uint32_t Attributes;

    //uint32_t DllNameRVA;                       // RVA to the name of the target library (NULL-terminate ASCII string)
    uint32_t Name;

    //uint32_t ModuleHandleRVA;                  // RVA to the HMODULE caching location (PHMODULE)
    uint32_t ModuleHandle;

    //uint32_t ImportAddressTableRVA;            // RVA to the start of the IAT (PIMAGE_THUNK_DATA)
    uint32_t DelayImportAddressTable;

    //uint32_t ImportNameTableRVA;               // RVA to the start of the name table (PIMAGE_THUNK_DATA::AddressOfData)
    uint32_t DelayImportNameTable;

    //uint32_t BoundImportAddressTableRVA;       // RVA to an optional bound IAT
    uint32_t BoundDelayImportTable;

    //uint32_t UnloadInformationTableRVA;        // RVA to an optional unload info table
    uint32_t UnloadDelayImportTable;

    //uint32_t TimeDateStamp;                    // 0 if not bound,
    uint32_t TimeStamp;
                                            // Otherwise, date/time of the target DLL
} delay_load_directory_record_t;


typedef struct {
    uint32_t Characteristics;
    uint32_t TimeDateStamp;
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint32_t Type;
    uint32_t SizeOfData;
    uint32_t AddressOfRawData;
    uint32_t PointerToRawData;
} debug_directory_record_t;

#define IMAGE_DEBUG_TYPE_UNKNOWN          0
#define IMAGE_DEBUG_TYPE_COFF             1
#define IMAGE_DEBUG_TYPE_CODEVIEW         2
#define IMAGE_DEBUG_TYPE_FPO              3
#define IMAGE_DEBUG_TYPE_MISC             4
#define IMAGE_DEBUG_TYPE_EXCEPTION        5
#define IMAGE_DEBUG_TYPE_FIXUP            6
#define IMAGE_DEBUG_TYPE_OMAP_TO_SRC      7
#define IMAGE_DEBUG_TYPE_OMAP_FROM_SRC    8
#define IMAGE_DEBUG_TYPE_BORLAND          9
#define IMAGE_DEBUG_TYPE_RESERVED10       10
#define IMAGE_DEBUG_TYPE_CLSID            11


typedef struct {
    uint32_t   Characteristics;
    uint32_t   TimeDateStamp;
    uint16_t    MajorVersion;
    uint16_t    MinorVersion;
    uint32_t   Name;
    uint32_t   Base;
    uint32_t   NumberOfFunctions;
    uint32_t   NumberOfNames;
    uint32_t   AddressOfFunctions;     // RVA from base of image
    uint32_t   AddressOfNames;         // RVA from base of image
    uint32_t   AddressOfNameOrdinals;  // RVA from base of image
} export_directory_record_t;


typedef struct {
    union {
        uint32_t   Characteristics;            // 0 for terminating null import descriptor
        uint32_t   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
    } DUMMYUNIONNAME;
    uint32_t   TimeDateStamp;                  // 0 if not bound,
                                            // -1 if bound, and real date\time stamp
                                            //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
                                            // O.W. date/time stamp of DLL bound to (Old BIND)

    uint32_t   ForwarderChain;                 // -1 if no forwarders
    uint32_t   Name;
    uint32_t   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
} IMAGE_IMPORT_DESCRIPTOR;
typedef import_directory_record_t;




int main()
{
}
