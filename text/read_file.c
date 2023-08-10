#include <stdio.h>
#include <stdbool.h>

#include <text_buffer/text_buffer.h>

#include "read_file.h"

struct TextResult readOpenedFile(FILE *in);

struct TextResult readFile(const char *path) {
    FILE *in = fopen(path, "r");

    if (in == NULL) {
        return (struct TextResult) {
            NULL,
            0
        };
    }

    return readOpenedFile(in);
}

struct TextResult readOpenedFile(FILE *in) {
    struct TextBuffer *result = TextBuffer_create();
    while (true) {
        int c = fgetc(in);
        if (c == EOF) {
            break;
        }
        TextBuffer_append(result, (char) c);
    }
    return TextBuffer_release(result);
}
