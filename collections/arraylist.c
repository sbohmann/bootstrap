#include <template_engine/template_engine.h>
#include <printf.h>
#include <stdlib.h>

#include "arraylist.h"
#include "text/read_file.h"

struct Configuration {
    const char *typeName;
};

void buildIntArrayList();

void buildArrayLists() {
    char *headerTemplatePath = "templates/arraylist.h.template";
    const char *headerTemplate = readFile(headerTemplatePath).text;
    if (headerTemplate == NULL) {
        fprintf(stderr, "Unable to read array list template from [%s]\n", headerTemplatePath);
        exit(1);
    }
    printf("%s\n", headerTemplate);
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
