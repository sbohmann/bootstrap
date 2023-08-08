#pragma once

struct Replacaments;

struct Replacements * Replacements_create();

void Replacements_delete(struct Replacements *instance);

void Replacements_add(struct Replacements *self, const char *key, const char *value);

const char * Replacements_get(struct Replacements *self, const char *key);
