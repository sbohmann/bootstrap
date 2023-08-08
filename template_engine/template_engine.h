#pragma once

#include "replacements.h"

struct TemplateEngineResult;

struct TemplateEngineResult * processTemplate(const char *template, struct Replacements *replacements);
