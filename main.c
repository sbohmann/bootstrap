#include <printf.h>
#include <stdlib.h>

#include "template_engine/template_engine.h"
#include "collections/arraylist.h"

void minitest();

int main() {
    minitest();
    buildArrayLists();
}

void minitest() {
    const char *template = "@__;abc@__key;@_key;def,@_x;,@_y;!@__";
    struct Replacements *replacements = Replacements_create();
    Replacements_add(replacements, "x", "xreplacement");
    Replacements_add(replacements, "key", "🥳");
    Replacements_add(replacements, "y", "yreplacement");
    Replacements_add(replacements, "x", "xreplacementadjusted");
    struct TextResult result =
            processTemplate(template,
            replacements);
    printf("Result: [%s]\n", result.text);
    free((void *) result.text);
    Replacements_delete(replacements);
}
