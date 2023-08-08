#include <stdlib.h>
#include <string.h>

#include "template_engine.h"

struct TextBuffer {
    char *data;
    size_t size;
    size_t capacity;
};

struct TextBuffer * TextBuffer_create() {
    struct TextBuffer *result = malloc(sizeof(struct TextBuffer));
    int initialCapacity = 256;
    result->data = malloc(initialCapacity);
    result->size = 0;
    result->capacity = initialCapacity;
    return result;
}

void TextBuffer_append(struct TextBuffer *buffer, char c) {
    if (buffer->size < buffer->capacity - 1) {
        buffer->data[buffer->size] = c;
        ++buffer->size;
    } else {
        size_t newDataCapacity = buffer->capacity * 2;
        char *newData = malloc(newDataCapacity);
        bzero(newData, newDataCapacity);
        memcpy(newData, buffer->data, buffer->size);
        char *oldData = buffer->data;
        buffer->data = newData;
        buffer->capacity = newDataCapacity;
        free(oldData);
    }
}

struct TemplateEngineResult bufferToResult(struct TextBuffer *buffer);

struct TemplateEngineResult processTemplate(const char *template, struct Replacements *replacements) {
    struct TextBuffer *buffer = TextBuffer_create();
    for (size_t index = 0; template[index] != 0; ++index) {
        const char c = template[index];
        TextBuffer_append(buffer, c);
    }
    return bufferToResult(buffer);
}

struct TemplateEngineResult bufferToResult(struct TextBuffer *buffer) {
    char *text = buffer->data;
    size_t textLength = buffer->size;
    free(buffer);
    return (struct TemplateEngineResult) {
        text,
        textLength
    };
}
