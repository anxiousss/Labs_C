#include "solution.h"


int read_line(FILE* fin, char **result, char end_char) {
    int buffer_size = 16;
    int length = 0;
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        return Memory_leak;
    }

    int ch;
    while ((ch = fgetc(fin)) != end_char && ch != EOF) {
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

    if (ch != '\n' && ch != EOF) {
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
    }

    return 0;
}

long long hash_func(String* def_name, int hash_size) {
    long long hash_value = 0;
    long long base_power = 1;

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

    return hash_value % hash_size;
}

HashNode* init_node(String* value, String* def_name) {
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (node == NULL) {
        return NULL;
    }
    copy_newstr(&node->value, value);
    copy_newstr(&node->def_name, def_name);
    node->next = NULL;
    node->length = 0;
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
    return 0;
}

void insert(HashTable * hash_table, String* value, String* def_name) {
    long long index = hash_func(def_name, hash_table->length);
    HashNode* new_node = init_node(value, def_name);

    if (hash_table->table[index] == NULL) {
        hash_table->table[index] = new_node;
    } else {
        new_node->next = hash_table->table[index];
        new_node->length = new_node->next->length + 1;
        hash_table->table[index] = new_node;
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
    for (int i = 0; i < hash_table->length; ++i) {
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

int restruct(HashTable* src, HashTable** dst, int capacity) {
    int err = init_hash_table(dst, capacity, src->length);

    if (err) {
        delete_hash_table(src);
        return Memory_leak;
    }

    for (int i = 0; i < src->length; ++i) {
        HashNode* tmp = src->table[i];
        while (tmp != NULL) {
            insert(*dst, &tmp->value, &tmp->def_name);
            tmp = tmp->next;
        }
    }
    delete_hash_table(src);
    return 0;
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
    int size = 0, err;
    while (1) {
        if (hash_table_check(hash_table)) {
            HashTable** dst = NULL;
            err = restruct(hash_table, dst, new_size(hash_table->capacity));
            if (err) {
                return Memory_leak;
            }
            hash_table = *dst;
        }
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
        insert(hash_table, &v, &s);
        ++size;
    }
    return 0;
}


