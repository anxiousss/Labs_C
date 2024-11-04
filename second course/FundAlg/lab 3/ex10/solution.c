#include "solution.h"

Node* init(char* value) {
    Node* root = (Node*)malloc(sizeof(Node));
    if (!root)
        return NULL;
    root->mas = NULL;
    root->value = strdup(value);
    if (!root->value) {
        free(root);
        return NULL;
    }
    root->size = 0;
    root->capacity = 0;
    return root;
}

int addChild(Node* parent, Node* child) {
    if (parent->size >= parent->capacity) {
        parent->capacity = (parent->capacity == 0) ? 1 : parent->capacity * 2;
        parent->mas = (Node**)realloc(parent->mas, parent->capacity * sizeof(Node *));
        if (parent->mas == NULL) {
            return Memory_leak;
        }
    }
    parent->mas[parent->size++] = child;
    return 0;
}

Node* build_tree(char* expression, int* index) {
    Node* root = init("");
    if (!root)
        return NULL;
    char buffer[100];
    int bufferIndex = 0;

    while (expression[*index] != '\0') {
        if (expression[*index] == '(') {
            (*index)++;
            Node* child = build_tree(expression, index);
            int err = addChild(root, child);
            if (err)
                return NULL;
        } else if (expression[*index] == ')') {
            (*index)++;
            if (bufferIndex > 0) {
                       buffer[bufferIndex] = '\0';
                root->value = strdup(buffer);
                if (!root->value) {
                    destroy(root);
                    return NULL;
                }
            }
            return root;
        }
        else {
            buffer[bufferIndex++] = expression[*index];
            (*index)++;
        }
    }

    if (bufferIndex > 0) {
        buffer[bufferIndex] = '\0';
        root->value = strdup(buffer);
        if (!root->value) {
            destroy(root);
            return NULL;
        }
    }
    return root;
}

void destroy(Node* root) {
    if (root == NULL)
        return;

    for (int i = 0; i < root->size; ++i) {
        destroy(root->mas[i]);
    }
    if (root->mas)
        free(root->mas);
    if (root->value)
        free(root->value);
    free(root);
}

void printTree(FILE* fin, Node* root, int level) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        fprintf(fin, "\t");
    }
    fprintf(fin, "%s\n", root->value);

    for (int i = 0; i < root->size; i++) {
        printTree(fin, root->mas[i], level + 1);
    }
}

char* readLineFromFile(FILE* file) {
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return NULL;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    char* line = strdup(buffer);
    if (line == NULL) {
        return NULL;
    }

    return line;
}
