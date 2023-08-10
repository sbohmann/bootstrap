#include <printf.h>

#include "template_engine/template_engine.h"

int main() {
    const char *template = "abc@_key;def,@_x;,@_y;!";
    struct Replacements *replacements = Replacements_create();
    Replacements_add(replacements, "x", "xreplacement");
    Replacements_add(replacements, "key", "🥳");
    Replacements_add(replacements, "y", "yreplacement");
    Replacements_add(replacements, "x", "xreplacementadjusted");
    struct TemplateEngineResult result =
            processTemplate(template,
            replacements);
    printf("Result: [%s]", result.text);
    Replacements_delete(replacements);
}
