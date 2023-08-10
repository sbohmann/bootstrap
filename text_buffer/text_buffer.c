#include <string.h>
#include "text_buffer.h"

struct TextBuffer {
    char *data;
    size_t size;
    size_t capacity;
};

struct TextBuffer *TextBuffer_create() {
    struct TextBuffer *result = malloc(sizeof(struct TextBuffer));
    int initialCapacity = 256;
    result->data = malloc(initialCapacity);
    result->size = 0;
    result->capacity = initialCapacity;
    return result;
}

void TextBuffer_delete(struct TextBuffer *instance) {
    free(instance->data);
    free(instance);
}

struct TextResult TextBuffer_get(struct TextBuffer *self) {
    return (struct TextResult) {
            self->data,
            self->size
    };
}

struct TextResult TextBuffer_release(struct TextBuffer *instance) {
    struct TextResult result = (struct TextResult) {
            instance->data,
            instance->size
    };
    free(instance);
    return result;
}

void TextBuffer_reset(struct TextBuffer *self) {
    self->size = 0;
}

void TextBuffer_append(struct TextBuffer *self, char c) {
    if (self->size + 1 > self->capacity - 1) {
        size_t newCapacity = self->capacity * 2;
        char *newData = malloc(newCapacity);
        bzero(newData, newCapacity);
        memcpy(newData, self->data, self->size);
        char *oldData = self->data;
        self->data = newData;
        self->capacity = newCapacity;
        free(oldData);
    }
    self->data[self->size] = c;
    self->data[self->size + 1] = 0;
    ++self->size;
}

void TextBuffer_appendBlock(struct TextBuffer *self, const char *block, size_t delta) {
    if (self->size + delta > self->capacity - 1) {
        size_t newCapacity = self->capacity * 2;
        while (self->size + delta > newCapacity - 1) {
            newCapacity *= 2;
        }
        char *newData = malloc(newCapacity);
        bzero(newData, newCapacity);
        memcpy(newData, self->data, self->size);
        char *oldData = self->data;
        self->data = newData;
        self->capacity = newCapacity;
        free(oldData);
    }
    memcpy(self->data + self->size, block, delta);
    self->data[self->size + delta] = 0;
    self->size += delta;
}

