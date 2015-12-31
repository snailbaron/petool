/*

 Initialization
 --------------

 Space characters at the start of the text are skipped, if present.
 line_start : Points to the first letter of the first word. Non-null, non-space character.
 line_end   : Points to the end of the first word. A space character or a null character.
 [line_start, line_end) fits the width (otherwise, the first word is too long).

 ...WORD...            ...WORD0
    ^   ^                 ^   ^
    |   |                 |   |
    |   +-- line_end      |   +-- line_end
    |                     | 
    +-- line_start        +-- line_start

 
 Working state
 -------------

 line_start : Points to a non-space, non-null character.
 line_end   : Points to a space character or null character.
 [line_start, line_end) fits the width.

 WORD ... WORD___             WORD ... WORD0
 ^            ^               ^              ^
 |            |               |              |
 + line_start +-- line_end    |              +-- line_end

 Searching next word:

 1. The next non-space character (null or non-null) from line_end is found.

  +-- line_start           +-- line_start           +-- line_start
 |                        |                           
 | line_end --+           | line_end --+            line_end --+
 |            |           |            |                       |
 v            v           v            v                       v
 WORD ... WORD___WORD     WORD ... WORD___0          WORD ... WORD0
                 ^                        ^                       ^    
                 |                        |                       |    
    word_start --+           word_start --+          word_start --+    

 If word_start points to null, finish line and text.

 2. Search the end of word

 +-- line_start                     +-- line_start
 |                                  |        
 | line_end --+                     | line_end --+
 |            |                     |            |
 v            v                     v            v
 WORD ... WORD___WORD___            WORD ... WORD___WORD0
                 ^   ^                              ^   ^
                 |   |                              |   |
    word_start --+   |                 word_start --+   |
                     |                                  |
          word_end --+                       word_end --+


 Evaluate [line_start, word_end). If fits:

 +-- line_start                     +-- line_start
 |                                  |
 |        line_end --+              |        line_end --+
 |                   |              |                   |
 v                   v              v                   v
 WORD ... WORD___WORD___            WORD ... WORD___WORD0

 If does not fit:

 +-- line_start                     +-- line_start
 |                                  |
 |        line_end --+              |       line_end --+
 |                   |              |                  |
 v                   v              v                  v
 WORD ... WORD___WORD___            WORD    WORD...WORD0


 Current state:
   line_start : Start of current line. A non-space non-null character.
   line_end   : Current end of line, a space or null character. [line_start, line_end) fits width.
 If line_end is not null, need to search next word.

 Search next word:
   1. Set word_start: beginning of next word. If null character, finish text.
   2. Set word_end: end of next word. May be a space or a null.
   3. Evaluate [line_start, word_end).
     3.a If fits, set line_end to word_end.
     3.b If does not fit, push current line [line_start, line_end), set line_start and line_end.
   

 When the text continues, and we should evalueate the next word into the line:
   cur_end    : end of current word
   next_word  : next word to be evaluated

 Begin text processing
 ---------------------

 ...WORD...WORD...WORD...0           ...0
    ^                                   ^
    |                                   |
    +- line_start                       +- line_start

  (more work to do)                  (finished)


 Finding first word of line
 --------------------------

 (more words follow)           (spaces and \0 follow)    (\0 immediately follows)

 WORD...WORD                   WORD...0                  WORD0
 ^   ^  ^                      ^   ^  ^                  ^   ^
 |   |  |                      |   |  |                  |   |\
 |   |  +-- cur_word           |   |  +-- cur_word       |   | -- cur_word
 |   |                         |   |                     |   |
 |   +-- line_end              |   +-- line_end          |   +-- line_end
 |                             |                         |
 +-- line_start                +-- line_start            +-- line_start

 Add more words to the line    Finish the line           Finish the line

 Check line_end - line_start fits. Otherwise, fail.

 Adding more words to the line
 -----------------------------

 Possible if next_word points to a non-null character.

 Find end:

 WORD...WORD...WORD           WORD...WORD...0              WORD...WORD0
 ^   ^  ^   ^  ^              ^   ^  ^   ^  ^              ^   ^  ^   ^
 |   |  |   |  |              |   |  |   |  |              |   |  |   |\
 |   |  |   |  +-- next_word  |   |  |   |  +-- next_word  |   |  |   | +-- next_word
 |   |  |   |                 |   |  |   |                 |   |  |   |
 |   |  |   +-- cur_end       |   |  |   +-- cur_end       |   |  |   +-- cur_end
 |   |  |                     |   |  |                     |   |  |
 |   |  +-- cur_word          |   |  +-- cur_word          |   |  +-- cur_word
 |   |                        |   |                        |   |
 |   +-- line_end             |   +-- line_end             |   +-- line_end
 |                            |                            |
 +-- line_start               +-- line_start               +-- line_start

 Check (cur_end - line_start) fits. If not, push current line:
   * Mark [line_start, line_end) as the line, or pass somewhere
   * line_start := cur_word
   * line_end   := cur_end
   * cur_word   := next_word

 WORD...WORD...WORD           WORD...WORD...0              WORD...WORD0
        ^   ^  ^                     ^   ^  ^                     ^   ^
        |   |  |                     |   |  |                     |   |\
        |   |  +-- cur_word          |   |  +-- cur_word          |   | +-- cur_word
        |   |                        |   |                        |   |
        |   +-- line_end             |   +-- line_end             |   +-- line_end
        |                            |                            |
        +-- line_start               +-- line_start               +-- line_start

 If (cur_end - line_start) fits, move the line_end and continue search:
   * line_end := cur_end
   * cur_word := next_word

 WORD...WORD...WORD           WORD...WORD...0              WORD...WORD0
 ^          ^  ^              ^          ^  ^              ^          ^
 |          |  |              |          |  |              |          |\
 |          |  +-- cur_word   |          |  +-- cur_word   |          | +-- cur_word 
 |          |                 |          |                 |          |
 |          +-- line_end      |          +-- line_end      |          +-- line_end
 |                            |                            |
 +-- line_start               +-- line_start               +-- line_start






*/


#include <stdbool.h>
#include <assert.h>
#include "text.h"

bool _is_space(char c)
{
    return (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v');
}

bool _is_space_or_end(char c)
{
    return (_is_space(c) || c == '\0');
}

void text_split(const char *text, size_t width)
{
//Current state:
//  line_start : Start of current line. A non-space non-null character.
//  line_end   : Current end of line, a space or null character. [line_start, line_end) fits width.
//If line_end is not null, need to search next word.

//Search next word:
//  1. Set word_start: beginning of next word. If null character, finish text.
//  2. Set word_end: end of next word. May be a space or a null.
//  3. Evaluate [line_start, word_end).
//    3.a If fits, set line_end to word_end.
//    3.b If does not fit, push current line [line_start, line_end), set line_start and line_end.

    const char *line_start = text;
    while (_is_space(*line_start)) {
        line_start++;
    }
    if (*line_start == '\0') {
        // TODO: empty text
    }

    const char *line_end = line_start;
    while (!_is_space(*line_end)) {
        line_end++;
    }
    if (*line_end == '\0') {
        // TODO: empty text
    }

    // TODO: Check [line_start, line_end). Otherwise, a word is too long.

    for (;;) {
        const char *word_start = line_end;
        while (_is_space(*word_start)) {
            word_start++;
        }
        if (*word_start == '\0') {
            // TODO: finish line and text
        }

        const char *word_end = word_start;
        while (!_is_space(*word_end)) {
            word_end++;
        }

        if (word_end - line_start <= width) {
            line_end = word_end;
        } else {
            // TODO: finish line [line_start, line_end)
            line_start = word_start;
            line_end = word_end;
            // TODO: Check [line_start, line_end). Otherwise, the word is too long.
        }
    }

   
        
}

