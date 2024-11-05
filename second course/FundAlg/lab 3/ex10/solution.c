#include "solution.h"


Node* create_node(char value) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
        return NULL;
    node->value = value;
    node->children = NULL;
    node->next = NULL;
    return node;
}

void add_child(Node* parent, Node* child) {
    if (parent->children == NULL) {
        parent->children = child;
    } else {
        Node* sibling = parent->children;
        while (sibling->next != NULL) {
            sibling = sibling->next;
        }
        sibling->next = child;
    }
}

Node* build_tree(const char* expression) {
    Node* stack[100];
    int stack_top = -1;
    Node* root = NULL;
    Node* current_node = NULL;
    int length = strlen(expression);
    for (int i = 0; i < length; i++) {
        char char_at_i = expression[i];

        if (char_at_i >= 'A' && char_at_i <= 'Z') {
            Node* new_node = create_node(char_at_i);
            if (!new_node) {
                destroy(root);
                return NULL;
            }
            if (current_node == NULL) {
                root = new_node;
                current_node = new_node;
            } else {
                add_child(current_node, new_node);
                current_node = new_node;
            }
        } else if (char_at_i == '(') {
            stack[++stack_top] = current_node;
        } else if (char_at_i == ')') {
            current_node = stack[stack_top--];
        } else if (char_at_i == ',') {
            current_node = stack[stack_top];
        }
    }

    return root;
}

void print_tree(Node* node, FILE * fin, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) {
        fprintf(fin, "\t");
    }
    fprintf(fin, "%c\n", node->value);

    Node* child = node->children;
    while (child != NULL) {
        print_tree(child, fin, level + 1);
        child = child->next;
    }
}

void destroy(Node* node) {
    if (node == NULL) return;

    Node* child = node->children;
    while (child != NULL) {
        Node* next_child = child->next;
        destroy(child);
        child = next_child;
    }

    free(node);
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

int check_file_names(const char *file1, const char *file2) {
    const char * name1 = strrchr(file1,'\\');
    const char * name2 = strrchr(file2,'\\');
    if (name1 != NULL) {
        name1++;
    } else {
        name1 = file1;
    }
    if (name2 != NULL) {
        name2++;
    } else {
        name2 = file2;
    }

    if (strcmp(name1, name2) == 0) {
        return Equal_Paths;
    }
    return 0;
}