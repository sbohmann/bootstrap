#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include <text_buffer/text_buffer.h>

#include "template_engine.h"

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
    const char *key = TextBuffer_get(self->keyBuffer).text;
    struct TextResult replacement = Replacements_get(self->replacements, key);
    if (replacement.text == NULL) {
        fprintf(stderr, "No replacement found for key [%s]", key);
        exit(1);
    }
    TextBuffer_appendBlock(self->buffer,
                           replacement.text,
                           replacement.size);
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
