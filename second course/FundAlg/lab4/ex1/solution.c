#include "solution.h"


int read_line(FILE* fin, char **result) {
    int buffer_size = 16;
    int length = 0;
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        return Memory_leak;
    }

    int ch;
    while ((ch = fgetc(fin)) && ch != ' ' && ch != '\n' && ch != EOF) {
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
    node->hash = 0;
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
        (*ht)->table[i] = init_node(NULL, NULL);
        if (!(*ht)->table[i]) {
            delete_hash_table(*ht);
            return Memory_leak;
        }
    }
    return 0;
}

void insert(HashTable * hash_table, HashNode* node) {
    if (node->hash == 0) {
        unsigned long hash = hash_func(&node->def_name);
        node->hash = hash;
    }
    unsigned long index = node->hash % hash_table->capacity;
    if (hash_table->table[index] == NULL) {
        hash_table->table[index] = node;
    } else {
        node->next = hash_table->table[index];
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
        min_length = min(min_length, hash_table->table[i]->length);
        max_length = max(max_length, hash_table->table[i]->length);
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

int read_define(FILE* fin, HashTable* hash_table) {
    int err;
    while (1) {
        if (hash_table_check(hash_table)) {
            HashTable* dst;
            err = init_hash_table(&dst, new_size(hash_table->capacity), 0);
            if (err) {
                delete_hash_table(hash_table);
                return err;
            }
            restruct(hash_table, &dst);
            hash_table = dst;
        }
        char* tmp = NULL;
        char* def_name = NULL;
        char* value = NULL;
        err = read_line(fin, &tmp);
        if (err) {
            return err;
        }
        if (strcmp(tmp, "#define") != 0) {
            break;
        }
        err = read_line(fin, &def_name);
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
        err = read_line(fin, &value);
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
        HashNode* node = init_node(&s, &v);
        if (!node) {
            free(tmp);
            free(def_name);
            free(value);
            return err;
        }
        insert(hash_table, node);
        printf("%s %s %s\n", tmp, def_name, value);
        free(tmp);
        free(def_name);
        free(value);
    }
    return 0;
}

int replacer(FILE* fin, HashTable* hash_table) {
    int err;
    while (!feof(fin)) {
        char* tmp = NULL;
        err = read_line(fin, &tmp);
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

    err = read_define(fin , ht);
    if (err) {
        delete_hash_table(ht);
        return err;
    }
    err = replacer(fin, ht);
    if (err) {
        delete_hash_table(ht);
        return err;
    }
    delete_hash_table(ht);
    return 0;
}