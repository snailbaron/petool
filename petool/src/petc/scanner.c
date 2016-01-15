#include <stdio.h>
#include <stdbool.h>

// Scanner state: file, character, and position
static FILE *file = NULL;
static int c = '\0';
static unsigned int line_number = 1;
static unsigned int sym_number = 0;

void scanner_init(const char *fname)
{
    fopen_s(&file, fname, "r");

}

int sym()
{
    return c;
}

bool is_char(char ch)
{
    return (c == ch);
}

bool is_letter()
{
    return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}

bool is_digit()
{
    return (c >= '0' && c <= '9');
}

bool is_ws()
{
    return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\n');
}

bool is_eof()
{
    return (c == EOF);
}

void scan()
{
    // Update line and symbol numbers for new character
    if (c == '\n') {
        line_number++;
        sym_number = 0;
    }
    sym_number++;

    // Get next character
    c = getc(file);

    // Convert all newlines to LF
    if (c == '\r') {
        c = getc(file);
        if (c != '\n') {
            fseek(file, -1, SEEK_CUR);
        }
        c = '\n';
    }
}

const char * char_to_str(int c)
{
    switch (c) {
        case  0: return "NUL (Null char)";
        case  1: return "SOH (Start of Heading)";
        case  2: return "STX (Start of Text)";
        case  3: return "ETX (End of Text)";
        case  4: return "EOT (End of Transmission)";
        case  5: return "ENQ (Enquiry)";
        case  6: return "ACK (Acknowledgment)";
        case  7: return "BEL (Bell)";
        case  8: return "BS (Back Space)";
        case  9: return "HT (Horizontal Tab)";
        case 10: return "LF (Line Feed)";
        case 11: return "VT (Vertical Tab)";
        case 12: return "FF (Form Feed)";
        case 13: return "CR (Carriage Return)";
        case 14: return "SO (Shift Out / X-On)";
        case 15: return "SI (Shift In / X-Off)";
        case 16: return "DLE (Data Line Escape)";
        case 17: return "DC1 (Device Control 1 (oft. XON))";
        case 18: return "DC2 (Device Control 2)";
        case 19: return "DC3 (Device Control 3 (oft. XOFF))";
        case 20: return "DC4 (Device Control 4)";
        case 21: return "NAK (Negative Acknowledgement)";
        case 22: return "SYN (Synchronous Idle)";
        case 23: return "ETB (End of Transmit Block)";
        case 24: return "CAN (Cancel)";
        case 25: return "EM (End of Medium)";
        case 26: return "SUB (Substitute)";
        case 27: return "ESC (Escape)";
        case 28: return "FS (File Separator)";
        case 29: return "GS (Group Separator)";
        case 30: return "RS (Record Separator)";
        case 31: return "US (Unit Separator)";
        case 32: return "Space";
        case 127: return "Delete";
        case EOF: return "EOF (End of File)";
    }

    if (c >= 33 && c <= 126) {
        static char ch;
        ch = c;
        return &ch;
    }

    static char buf[100];
    sprintf_s(buf, 100, "%d (0x%x)", c, c);
    return buf;
}
