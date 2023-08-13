#pragma once

#include "replacements.h"

#include "text/text_result.h"

struct TextResult processTemplate(const char *template, struct Replacements *replacements);
