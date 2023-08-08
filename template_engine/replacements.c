#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "replacements.h"

struct Node {
    const char *key;
    const char *value;
    struct Node *left;
    struct Node *right;
};

struct Replacements {
    struct Node *baseNode;
};

struct Replacements * Replacements_create() {
    struct Replacements *result = malloc(sizeof(struct Replacements));
    bzero(result, sizeof(struct Replacements));
    return result;
}

void Replacements_delete(struct Replacements *instance) {
    free(instance);
}

void insert(struct Node *node, const char *key, const char *value);

void Replacements_add(struct Replacements *self, const char *key, const char *value) {
    if (self->baseNode == NULL) {
        struct Node *newBaseNode = malloc(sizeof(struct Node));
        newBaseNode->key = key;
        self->baseNode = newBaseNode;
    } else {
        insert(self->baseNode, key, value);
    }
}

const char *valueForNode(struct Node *pNode, const char *key);

const char * Replacements_get(struct Replacements *self, const char *key) {
    return valueForNode(self->baseNode, key);
}

const char *valueForNode(struct Node *node, const char *key) {
    if (node == NULL) {
        return NULL;
    }
    int delta = strcmp(key, node->key);
    if (delta == 0) {
        return node->value;
    } else if (delta < 0) {
        return valueForNode(node->left, key);
    } else {
        return valueForNode(node->right, key);
    }
}

struct Node *createNode(const char *key, const char *value);

void insert(struct Node *node, const char *key, const char *value) {
    int delta = strcmp(key, node->key);
    if (delta == 0) {
        node->value = value;
    } else if (delta < 0) {
        if (node->left != NULL) {
            insert(node->left, key, value);
        } else {
            node->left = createNode(key, value);
        }
    } else {
        if (node->right != NULL) {
            insert(node->right, key, value);
        } else {
            node->left = createNode(key, value);
        }
    }
}

struct Node *createNode(const char *key, const char *value) {
    struct Node *result = malloc(sizeof(struct Node));
    bzero(result, sizeof(struct Node));
    result->key = key;
    result->value = value;
    return result;
}
