#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "template_engine.h"

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

enum state {
    text_state,
    at_state,
    underscore_state,
    key_state
};

struct TemplateEngine {
    const char *template;
    struct Replacements *replacements;
    struct TextBuffer *buffer;
    struct TextBuffer *keyBuffer;
    enum state state;
    size_t outputIndex;
};

void TemplateEngine_writeToIndex(struct TemplateEngine *self, size_t index);

struct TextResult bufferToResult(struct TextBuffer *buffer);

void TemplateEngine_writeKeyReplacement(struct TemplateEngine *self);

bool isKeyChar(char c);

bool isKeyStartChar(char c);

struct TextResult processTemplate(const char *template, struct Replacements *replacements) {
    struct TemplateEngine self;
    self.template = template;
    self.replacements = replacements;
    self.buffer = TextBuffer_create();
    self.keyBuffer = TextBuffer_create();
    self.state = text_state;
    self.outputIndex = 0;
    size_t index;
    for (index = 0; template[index] != 0; ++index) {
        const char c = template[index];
        switch (self.state) {
            case text_state:
                if (c == '@') {
                    self.state = at_state;
                } else {
                    TextBuffer_append(self.buffer, c);
                    self.outputIndex = index + 1;
                }
                break;
            case at_state:
                if (c == '_') {
                    self.state = underscore_state;
                } else if (c == '@') {
                    TemplateEngine_writeToIndex(&self, index);
                    self.state = at_state;
                } else {
                    TemplateEngine_writeToIndex(&self, index);
                    self.state = text_state;
                }
                break;
            case underscore_state:
                if (isKeyStartChar(c)) {
                    TextBuffer_append(self.keyBuffer, c);
                    self.state = key_state;
                } else if (c == '@') {
                    TemplateEngine_writeToIndex(&self, index);
                    self.state = at_state;
                } else if (c == '_') {
                    TextBuffer_appendBlock(self.buffer, "@_", 2);
                    self.outputIndex = index + 1;
                    self.state = text_state;
                } else {
                    TemplateEngine_writeToIndex(&self, index);
                    self.state = text_state;
                }
                break;
            case key_state:
                if (isKeyChar(c)) {
                    TextBuffer_append(self.keyBuffer, c);
                } else if (c == '@') {
                    TemplateEngine_writeToIndex(&self, index);
                    self.state = at_state;
                } else if (c == ';') {
                    TemplateEngine_writeKeyReplacement(&self);
                    self.outputIndex = index + 1;
                    self.state = text_state;
                }
        }
    }
    TemplateEngine_writeToIndex(&self, index);
    TextBuffer_delete(self.keyBuffer);
    return TextBuffer_release(self.buffer);
}

void TemplateEngine_writeToIndex(struct TemplateEngine *self, size_t index) {
    size_t delta = index - self->outputIndex;
    TextBuffer_appendBlock(self->buffer, self->template + self->outputIndex, delta);
    self->outputIndex = index;
}

void TemplateEngine_writeKeyReplacement(struct TemplateEngine *self) {
    char *key = self->keyBuffer->data;
    struct Replacement replacement = Replacements_get(self->replacements, key);
    if (replacement.value == NULL) {
        fprintf(stderr, "No replacement found for key [%s]", key);
        exit(1);
    }
    TextBuffer_appendBlock(self->buffer,
                           replacement.value,
                           replacement.length);
    TextBuffer_reset(self->keyBuffer);
}

bool isKeyChar(char c) {
    return c == '-' ||
           c == '_' ||
           isKeyStartChar(c);
}

bool isKeyStartChar(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9');
}
