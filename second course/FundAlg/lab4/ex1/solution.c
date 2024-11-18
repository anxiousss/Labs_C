#include "solution.h"

DirectiveList* init_directive(void) {
    DirectiveList* list = (DirectiveList*)(malloc(sizeof(DirectiveList)));
    if (!list)
        return NULL;

    list->head = (DirectiveNode*)(malloc(sizeof(DirectiveNode)));
    if (!list->head) {
        free(list);
        return NULL;
    }

    int err = init_string(&list->head->value, NULL);
    if (err) {
        free(list->head);
        free(list);
        return NULL;
    }
    err = init_string(&list->head->def_name, NULL);
    if (err) {
        delete_string(&list->head->value);
        free(list);
        return NULL;
    }
    list->head->next = NULL;
    return list;
}

int directive_push(DirectiveList* list, String* def_name, String* value) {
    if (list) {
        DirectiveNode* node = (DirectiveNode* )(malloc(sizeof(DirectiveNode)));
        if (!node) {
            return Memory_leak;
        }

        node->value = *value;
        node->def_name = *def_name;

        node->next = NULL;
        if (!list->head) {
            list->head = node;
        } else {
            DirectiveNode* iterator = list->head;
            while (iterator->next != NULL) {
                iterator = iterator->next;
            }
            iterator->next = node;
        }
    }
    return 0;
}

void directive_destroy(DirectiveList* list) {
    DirectiveNode* tmp = list->head;
    while (tmp != NULL) {
        delete_string(&tmp->value);
        delete_string(&tmp->def_name);
        DirectiveNode* tmp2 = tmp;
        tmp = tmp->next;
        free(tmp2);
    }
    free(list);
}


int read_line(FILE* fin, char **result, char end_char) {
    int buffer_size = 16;
    int length = 0;
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        return Memory_leak;
    }

    int ch;
    while ((ch = fgetc(fin)) && ch != end_char && ch != EOF) {
        if (length + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);

            if (!new_buffer) {
                free(buffer);
                return Memory_leak;
            }

            buffer = new_buffer;
        }

        buffer[length++] = ch;
    }

    buffer[length] = '\0';

    *result = buffer;

    return 0;
}

unsigned long hash_func(String* def_name) {
    unsigned long hash_value = 0;
    unsigned long base_power = 1;

    for (int i = def_name->size - 1; i >= 0; i--) {
        char c = def_name->mas[i];
        int digit;

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 36;
        } else {
            return Invalid_input;
        }

        hash_value += digit * base_power;
        base_power *= 62;
    }

    return hash_value;
}

HashNode* init_node(String* value, String* def_name) {
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (node == NULL) {
        return NULL;
    }

    if (value == NULL || def_name == NULL) {
        if (value == NULL) init_string(&node->value, NULL);
        if (def_name == NULL) init_string(&node->def_name, NULL);
        node->next = NULL;
        node->length = 1;
        return node;
    }


    copy_newstr(&node->value, value);
    copy_newstr(&node->def_name, def_name);
    node->next = NULL;
    node->length = 1;
    node->hash = hash_func(def_name);
    return node;
}



int init_hash_table(HashTable** ht, int capacity, int length) {
    *ht = (HashTable *)(malloc(sizeof(HashTable)));
    if (!*ht)
        return Memory_leak;
    (*ht)->table = (HashNode**)calloc(capacity, sizeof(HashNode*));
    if ((*ht)->table == NULL) {
        free(*ht);
        return Memory_leak;
    }
    (*ht)->length = length;
    (*ht)->capacity = capacity;
    for (int i = 0; i < capacity; ++i) {
        (*ht)->table[i] = NULL;
    }
    return 0;
}

void insert(HashTable * hash_table, HashNode* node) {
    unsigned long index = node->hash % hash_table->capacity;
    if (hash_table->table[index] == NULL) {
        hash_table->table[index] = node;
    } else {
        node->next = init_node(&hash_table->table[index]->value, &hash_table->table[index]->def_name);
        node->length = node->next->length + 1;
        hash_table->table[index] = node;
    }
    hash_table->length++;
}

void delete_node(HashNode* hashNode) {
    HashNode* tmp1 = hashNode;
    while (tmp1 != NULL) {
        delete_string(&tmp1->value);
        delete_string(&tmp1->def_name);
        HashNode* tmp2 = tmp1;
        tmp1 = tmp1->next;
        free(tmp2);
    }
}

void delete_hash_table(HashTable* hash_table) {
    for (int i = 0; i < hash_table->capacity; ++i) {
        delete_node(hash_table->table[i]);
    }
    free(hash_table);
}

int new_size(int hash_size) {
    return hash_size * 2;
}

int hash_table_check(HashTable* hash_table) {
    int min_length = INT_MAX;
    int max_length = INT_MIN;

    for (int i = 0; i < hash_table->capacity; ++i) {
        if (hash_table->table[i]) {
            min_length = min(min_length, hash_table->table[i]->length);
            max_length = max(max_length, hash_table->table[i]->length);
        }
    }
    if (max_length / min_length >= 2) {
        return 1;
    }
    return 0;
}

void restruct(HashTable* src, HashTable** dst) {
    for (int i = 0; i < src->capacity; ++i) {
        HashNode* tmp = src->table[i];
        while (tmp != NULL) {
            insert(*dst, tmp);
            tmp = tmp->next;
        }
    }

    delete_hash_table(src);
}

int is_correct_def_name(String *s) {
    for (int i = 0; i < s->size; ++i) {
        if ((s->mas[i] < '0' || s->mas[i] > '9') && (s->mas[i] < 'a' || s->mas[i] > 'z') &&
            (s->mas[i] < 'A' || s->mas[i] > 'Z')) {
            return 0;
        }
    }
    return 1;
}

int read_define(FILE* fin, DirectiveList* directives) {
    int err;
    while (1) {
        char* tmp = NULL;
        char* def_name = NULL;
        char* value = NULL;
        err = read_line(fin, &tmp, ' ');
        if (err) {
            return err;
        }
        if (strcmp(tmp, "#define") != 0) {
            break;
        }
        err = read_line(fin, &def_name, ' ');
        if (err) {
            free(tmp);
            return err;
        }
        String s;
        err = init_string(&s, &def_name);
        if (err) {
            free(tmp);
            return err;
        }
        if (!is_correct_def_name(&s)) {
            free(tmp);
            return Invalid_input;
        }
        err = read_line(fin, &value, '\n');
        if (err) {
            free(tmp);
            free(def_name);
            return err;
        }
        String v;
        err = init_string(&v, &value);
        if (err) {
            free(tmp);
            free(def_name);
            free(value);
            return err;
        }
        err = directive_push(directives, &s, &v);
        if (err) {
            free(tmp);
            free(def_name);
            free(value);
            return err;
        }
        printf("%s %s %s\n", tmp, def_name, value);
        free(tmp);
    }
    return 0;
}

int fill_hashtable(HashTable* hashTable, DirectiveList* list) {
    DirectiveNode* head = list->head;
    while (head != NULL) {
        if (hash_table_check(hashTable)) {
            HashTable* dst;
            int err = init_hash_table(&dst, new_size(hashTable->capacity), 0);
            if (err)
                return Memory_leak;
            restruct(hashTable, &dst);
            hashTable = dst;
        }
        HashNode* node = init_node(&head->value, &head->def_name);
        if (!node) {
            return Memory_leak;
        }
        insert(hashTable, node);
        head = head->next;
    }
    return 0;
}

int replacer(FILE* fin, HashTable* hash_table) {
    int err;
    while (!feof(fin)) {
        char* tmp = NULL;
        err = read_line(fin, &tmp, ' ');
        if (err)
            return err;
        String s;
        err = init_string(&s, &tmp);
        if (err)
            return err;

        for (int i = 0; i < hash_table->capacity; ++i) {
            HashNode* hashNode = hash_table->table[i];
            int key = 0;
            while (hashNode != NULL) {
                if (eq_string(&s, &hashNode->def_name)) {
                    copy_str(&s, &hashNode->value);
                    ungetc(s.size, fin);
                    fwrite(s.mas, sizeof(char ), s.size, fin);
                    key = 1;
                    break;
                }
                hashNode = hashNode->next;
            }
            if (key)
                break;
        }
        delete_string(&s);
    }
    return 0;
}


int text_replace(FILE* fin) {
    HashTable* ht;
    int err = init_hash_table(&ht, TABLE_SIZE, 0);
    if (err)
        return err;

    DirectiveList* list = init_directive();
    if (!list) {
        delete_hash_table(ht);
        return Memory_leak;
    }

    err = read_define(fin , list);
    if (err) {
        delete_hash_table(ht);
        directive_destroy(list);
        return err;
    }

    err = fill_hashtable(ht, list);
    if (err) {
        delete_hash_table(ht);
        directive_destroy(list);
        return err;
    }
    err = replacer(fin, ht);
    if (err) {
        delete_hash_table(ht);
        directive_destroy(list);
        return err;
    }
    delete_hash_table(ht);
    directive_destroy(list);
    return 0;
}