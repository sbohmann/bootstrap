#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "replacements.h"

struct Node {
    const char *key;
    const char *value;
    size_t length;
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

struct Node *createNode(const char *key, const char *value);

void Replacements_add(struct Replacements *self, const char *key, const char *value) {
    if (self->baseNode == NULL) {
        self->baseNode = createNode(key, value);
    } else {
        insert(self->baseNode, key, value);
    }
}

struct Replacement replacementForNode(struct Node *pNode, const char *key);

struct Replacement Replacements_get(struct Replacements *self, const char *key) {
    return replacementForNode(self->baseNode, key);
}

struct Replacement replacementForNode(struct Node *node, const char *key) {
    if (node == NULL) {
        return (struct Replacement) {
            NULL,
            0
        };
    }
    int delta = strcmp(key, node->key);
    if (delta == 0) {
        return (struct Replacement) {
            node->value,
            node->length
        };
    } else if (delta < 0) {
        return replacementForNode(node->left, key);
    } else {
        return replacementForNode(node->right, key);
    }
}

void insert(struct Node *node, const char *key, const char *value) {
    int delta = strcmp(key, node->key);
    if (delta == 0) {
        node->value = value;
        node->length = strlen(value);
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
    result->length = strlen(value);
    return result;
}
