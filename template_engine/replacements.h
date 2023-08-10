#pragma once

#include <stddef.h>

struct Replacaments;

struct Replacement {
    const char *value;
    size_t length;
};

struct Replacements * Replacements_create();

void Replacements_delete(struct Replacements *instance);

void Replacements_add(struct Replacements *self, const char *key, const char *value);

struct Replacement Replacements_get(struct Replacements *self, const char *key);
