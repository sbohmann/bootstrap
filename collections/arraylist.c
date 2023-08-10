#include <template_engine/template_engine.h>

#include "arraylist.h"

struct Configuration {
    const char *typeName;
};

void buildIntArrayList();

void buildArrayLists() {
    buildIntArrayList();
}

void buildArrayList(struct Configuration configuration);

void buildIntArrayList() {
    buildArrayList((struct Configuration) {
        "int"
    });
}

void buildArrayList(struct Configuration configuration) {
    struct Replacements *replacements = Replacements_create();
    Replacements_add(replacements, "typeName", configuration.typeName);
}
