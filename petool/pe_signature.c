#include <stdint.h>
#include "pe_signature.h"
#include "error.h"

bool read_pe_signature(FILE *infile)
{
    if (fseek(infile, 0x3C, SEEK_SET)) {
        set_error("Failed to go to 0x3C");
        return false;
    }

    uint32_t pe_sig_offset;
    if (fread(&pe_sig_offset, 4, 1, infile) < 1) {
        set_error("Failed to read PE signature offset");
        return false;
    }

    if (fseek(infile, pe_sig_offset, SEEK_SET)) {
        set_error("Failed to seek PE signature");
        return false;
    }

    uint8_t pe_sig_sample[4] = "PE\0\0";
    uint8_t pe_sig[4];
    if (fread(&pe_sig, 1, 4, infile) < 4) {
        set_error("Failed to read PE signature");
        return false;
    }

    for (size_t i = 0; i < 4; i++) {
        if (pe_sig[i] != pe_sig_sample[i]) {
            set_error("PE signature does not match");
            return false;
        }
    }
    
    return true;
}
