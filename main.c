#include <printf.h>

#include "template_engine/template_engine.h"

int main() {
    const char *template = "abc@_key;def";
    struct Replacements *replacements = Replacements_create();
    struct TemplateEngineResult result =
            processTemplate(template,
            replacements);
    printf("Result: [%s]", result.text);
    Replacements_delete(replacements);
}
