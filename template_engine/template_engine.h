#pragma once

#include <text/text_result.h>

#include "replacements.h"

struct TextResult processTemplate(const char *template, struct Replacements *replacements);
