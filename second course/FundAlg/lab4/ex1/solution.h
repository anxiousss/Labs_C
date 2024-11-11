

#ifndef EX4_1_SOLUTION_H
#define EX4_1_SOLUTION_H

#define TABLE_SIZE 128

#include "string.h"

typedef struct HashNode {
    String value;
    String def_name;
    long long hash;
    int length;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode** table;
    int length;
    int capacity;
} HashTable;

#define max(a, b) (a > b) ? a : b
#define min(a, b) (a < b) ? a : b

int read_line(FILE* fin, char **result, char end_char);

long long hash_func(String* string, int hash_size);

HashNode* init_node(String* value, String* def_name);

int init_hash_table(HashTable** hashTable, int capacity, int length);

void insert(HashTable* hashTable, String* value, String* def_name);

void delete_node(HashNode* hashNode);

void delete_hash_table(HashTable* hashTable);

int hash_table_check(HashTable* hashTable);

int new_size(int hash_size);

int restruct(HashTable* src, HashTable** dst, int capacity);

int is_correct_def_name(String *s);

int read_define(FILE* fin, HashTable* hash_table);

int replace(String* def_name, String* value);

int replacer(FILE* fin, HashTable* hash_table);

#endif //EX4_1_SOLUTION_H
