#pragma once

#include "replacements.h"

struct TextResult {
    const char *text;
    const size_t size;
};

struct TextResult processTemplate(const char *template, struct Replacements *replacements);
