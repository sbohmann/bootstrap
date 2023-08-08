#pragma once

#include "replacements.h"

struct TemplateEngineResult {
    const char *text;
    const size_t size;
};

struct TemplateEngineResult processTemplate(const char *template, struct Replacements *replacements);
