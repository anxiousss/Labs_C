

#ifndef EX4_1_SOLUTION_H
#define EX4_1_SOLUTION_H

#define TABLE_SIZE 128

#include "string.h"

typedef struct HashNode {
    String value;
    String def_name;
    int length;
    struct HashNode* next;
    unsigned long hash;
} HashNode;

typedef struct {
    HashNode** table;
    int length;
    int capacity;
} HashTable;

#define max(a, b) (a > b) ? a : b
#define min(a, b) (a < b) ? a : b

int read_line(FILE* fin, char **result);

unsigned long hash_func(String* string);

HashNode* init_node(String* value, String* def_name);

int init_hash_table(HashTable** hashTable, int capacity, int length);

void insert(HashTable* hashTable, HashNode* hashNode);

void delete_node(HashNode* hashNode);

void delete_hash_table(HashTable* hashTable);

int hash_table_check(HashTable* hashTable);

int new_size(int hash_size);

void restruct(HashTable* src, HashTable** dst);

int is_correct_def_name(String *s);

int read_define(FILE* fin, HashTable* hash_table);

int replacer(FILE* fin, HashTable* hash_table);

int text_replace(FILE* fin);

#endif //EX4_1_SOLUTION_H
