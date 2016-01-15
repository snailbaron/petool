#include "../vis_struct.h"
#include "../label.h"

// Lexer state

typedef enum
{
    TOKEN_NUMBER,
    TOKEN_WORD,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_COMMA,
    TOKEN_HORIZ_SEP,
    TOKEN_VERT_SEP,
    TOKEN_LEFT_BR,
    TOKEN_RIGHT_BR,
} token_type_t;

typedef struct
{
    token_type_t type;
    vis_value_t value;
} token_number_t;

typedef struct
{
    token_type_t type;
    label_t word;
} token_word_t;

typedef struct
{
    token_type_t type;
    label_t string;
} token_string_t;

typedef union
{
    token_type_t type;
    token_number_t number;
    token_word_t word;
    token_string_t string;
} token_t;

static token_t token;

void lex()
{
    // Get token under cursor
    if (is_digit())
    {
        token.type = TOKEN_NUMBER;
        label_t buf = label_init();
        while (is_digit()) {
            label_append_char(&buf, sym());
            scan();
        }
        token.number.value = strtoull(buf.str, NULL, 10);
    } else if (is_letter() || is_char('_')) {
        token.type = TOKEN_WORD;
        while (is_letter() || is_digit() || is_char('_') || is_char('+')) {
            label_append_char(&token.word.word, sym());
            scan();
        }
    } else if (is_char('[')) {
        token.type = TOKEN_STRING;
        while (is_ws()) {
            scan();
        }
        while (!is_char(']') && !is_eof()) {
            label_append_char(&token.string.string, sym());
            scan();
        }

        if (is_eof()) {
            // TODO: Report lexer error
        }
    } else if (is_char('-')) {
        token.type = TOKEN_HORIZ_SEP;
        while (is_char('-')) {
            scan();
        }
    } else if (is_eof()) {
        token.type = TOKEN_EOF;
    } else if (is_char(',')) {
        token.type = TOKEN_COMMA;
        scan();
    } else if (is_char('|')) {
        token.type = TOKEN_VERT_SEP;
        scan();
    } else {
        // TODO: Report lexer error
    }

    // Skip whitespace after obtained token
    while (is_ws()) {
        scan();
    }
}

