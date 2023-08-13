#pragma once

#include <stddef.h>

#include "text/text_result.h"

struct Replacaments;

struct Replacements * Replacements_create();

void Replacements_delete(struct Replacements *instance);

void Replacements_add(struct Replacements *self, const char *key, const char *value);

struct TextResult Replacements_get(struct Replacements *self, const char *key);
