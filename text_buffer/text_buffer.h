#pragma once

#include <stdlib.h>

#include <text/text_result.h>

struct TextBuffer;

struct TextBuffer *TextBuffer_create();

void TextBuffer_delete(struct TextBuffer *instance);

struct TextResult TextBuffer_get(struct TextBuffer *self);

struct TextResult TextBuffer_release(struct TextBuffer *instance);

void TextBuffer_reset(struct TextBuffer *self);

void TextBuffer_append(struct TextBuffer *self, char c);

void TextBuffer_appendBlock(struct TextBuffer *self, const char *block, size_t delta);
