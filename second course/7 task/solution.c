#include "solution.h"


int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}


int base_conversion(int number, char* result, int based) {
    if (based < 2 || based > 36) {
        result[0] = '\0';
        return Invalid_input;
    }

    if (number == 0) {
        result[0] = '0';
        result[1] = '\0';
        return Invalid_input;
    }

    char tmp[65];
    int index = 0;

    while (number > 0) {
        int remainder = number % based;
        if (remainder < 10) {
            tmp[index] = '0' + remainder;
        } else {
            tmp[index] = 'A' + (remainder - 10);
        }
        index++;
        number /= based;
    }

    for (int i = 0; i < index; i++) {
        result[i] = tmp[index - i - 1];
    }
    result[index] = '\0';
    return 0;
}

void free_buffer(WordsBuffer* buffer) {
    if (buffer == NULL)
        return;

    for (int i = 0; i < buffer->size; i++) {
        free(buffer->words[i]);
    }

    free(buffer->words);
    free(buffer->word_sizes);
    free(buffer->word_capacities);
    free(buffer);
}


WordsBuffer* create_buffer() {
    WordsBuffer* buffer = (WordsBuffer*)malloc(sizeof(WordsBuffer));
    if (buffer == NULL)
        return NULL;

    buffer->capacity = 4;
    buffer->size = 0;

    buffer->words = (char**)malloc(sizeof(char*) * buffer->capacity);
    buffer->word_sizes = (int*)malloc(sizeof(int) * buffer->capacity);
    buffer->word_capacities = (int*)malloc(sizeof(int) * buffer->capacity);

    if (buffer->words == NULL || buffer->word_sizes == NULL || buffer->word_capacities == NULL) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

int insert_symbol(WordsBuffer* buffer, char c) {
    if (buffer == NULL || buffer->size == 0)
        return Memory_leak;

    int last_i = buffer->size - 1;

    char* last_word = buffer->words[last_i];
    int len = buffer->word_sizes[last_i];
    int capacity = buffer->word_capacities[last_i];

    if (len == capacity) {
        buffer->word_capacities[last_i] *= 2;
        char* new_word = (char*)realloc(last_word, sizeof(char) * buffer->word_capacities[last_i]);

        if (new_word == NULL) {
            return Memory_leak;
        }

        buffer->words[last_i] = new_word;
    }

    buffer->words[last_i][buffer->word_sizes[last_i]] = c;
    buffer->word_sizes[last_i]++;

    return 0;
}

int insert_word(WordsBuffer* buffer) {
    if (buffer == NULL)
        return Memory_leak;

    if (buffer->size == buffer->capacity) {
        buffer->capacity *= 2;
        char** new_words = (char**)realloc(buffer->words, sizeof(char*) * buffer->capacity);
        int* new_word_sizes = (int*)realloc(buffer->word_sizes, sizeof(int) * buffer->capacity);
        int* new_word_capacities = (int*)realloc(buffer->word_capacities, sizeof(int) * buffer->capacity);

        if (new_words == NULL || new_word_sizes == NULL || new_word_capacities == NULL) {
            return Memory_leak;
        }

        buffer->words = new_words;
        buffer->word_sizes = new_word_sizes;
        buffer->word_capacities = new_word_capacities;
    }

    buffer->word_capacities[buffer->size] = 4;
    buffer->word_sizes[buffer->size] = 0;

    buffer->words[buffer->size] = (char*)malloc(sizeof(char) * buffer->word_capacities[buffer->size]);
    if (buffer->words[buffer->size] == NULL)
        return Memory_leak;

    buffer->size++;
    return 0;
}

int split_by_words(WordsBuffer* buffer, FILE* file) {
    if (buffer == NULL || file == NULL)
        return Memory_leak;

    char c;
    int err;
    while ((c = fgetc(file)) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (buffer->size > 0) {
                err = insert_symbol(buffer, '\0');
                if (err == Memory_leak)
                    return err;

            }
            err = insert_word(buffer);
            if (err == Memory_leak)
                return err;
        } else {
            if (buffer->size == 0) {
                err = insert_word(buffer);
                if (err == Memory_leak)
                    return err;
            }
            err = insert_symbol(buffer, c);
            if (err == Memory_leak)
                return err;
        }
    }

    if (buffer->size > 0) {
        insert_symbol(buffer, '\0');
    }

    return 0;
}
int HandlerOptR(char** paths) {
    if (strcmp(paths[0], paths[1]) == 0) {
        return Equal_paths;
    }

    FILE* fin1 = fopen(paths[0], "r");
    FILE* fin2 = fopen(paths[1], "r");
    FILE* out = fopen(paths[2], "w");

    if (!fin1 || !fin2 || !out) {
        if (fin1) fclose(fin1);
        if (fin2) fclose(fin2);
        if (out) fclose(out);
        return Memory_leak;
    }

    WordsBuffer* words_buffer1 = create_buffer();
    WordsBuffer* words_buffer2 = create_buffer();
    if (words_buffer1 == NULL || words_buffer2 == NULL) {
        return Memory_leak;
    }
    int res1 = split_by_words(words_buffer1, fin1);
    int res2 = split_by_words(words_buffer2, fin2);

    if (res1 != 0 || res2 != 0) {
        free_buffer(words_buffer1);
        free_buffer(words_buffer2);
        return Memory_leak;
    }

    int n_words1 = words_buffer1->size;
    int n_words2 = words_buffer2->size;

    int max_words = n_words1 > n_words2 ? n_words1 : n_words2;

    for (int i = 0; i < max_words; ++i) {

        if (i < n_words1 && i < n_words2) {
            if (i % 2 == 0) {
                fprintf(out, "%s ", words_buffer1->words[i]);
            } else {
                fprintf(out, "%s ", words_buffer2->words[i]);
            }
        }
        else if (i < n_words1) {
            fprintf(out, "%s ",words_buffer1->words[i]);
        }
        else if (i < n_words2) {
            fprintf(out, "%s ", words_buffer2->words[i]);
        }
    }

    free_buffer(words_buffer1);
    free_buffer(words_buffer2);
    fclose(fin1);
    fclose(fin2);
    fclose(out);
    return 0;
}

int HandlerOptA(char** paths) {
    if (strcmp(paths[0], paths[1]) == 0) {
        return Equal_paths;
    }

    FILE* fin1 = fopen(paths[0], "r");
    FILE* out = fopen(paths[1], "w");

    if (!fin1 || !out) {
        if (fin1) fclose(fin1);
        if (out) fclose(out);
        return Memory_leak;
    }


    WordsBuffer* words_buffer1 = create_buffer();
    
    if (words_buffer1 == NULL) {
        return Memory_leak;
    }
    int res1 = split_by_words(words_buffer1, fin1);

    if (res1 != 0) {
        free_buffer(words_buffer1);
        return Memory_leak;
    }
    
    // logic

    for (int i = 0; i < words_buffer1->size; ++i) {
        if (i % 10 == 9) {
            for (int j = 0; words_buffer1->words[i][j] != '\0'; ++j) {
                if (words_buffer1->words[i][j] >= 'A' && words_buffer1->words[i][j] <= 'Z')
                    words_buffer1->words[i][j] = 'a' + (words_buffer1->words[i][j] - 'A');
            }
            for (int j = 0; j < words_buffer1->words[i][j] != '\0'; ++j) {
                char result[65];
                int err = base_conversion(words_buffer1->words[i][j], result, 4);
                if (err) {
                    free_buffer(words_buffer1);
                    return err;
                }
                fprintf(out, "%s", result);
                putc(' ', out);
            }
        } else if (i % 2 == 1) {
            for (int j = 0; words_buffer1->words[i][j] != '\0'; ++j) {
                if (words_buffer1->words[i][j] >= 'A' && words_buffer1->words[i][j] <= 'Z')
                    words_buffer1->words[i][j] = 'a' + (words_buffer1->words[i][j] - 'A');
            }
            fprintf(out, "%s ", words_buffer1->words[i]);
        } else if (i % 5 == 4) {
            for (int j = 0; j < words_buffer1->words[i][j] != '\0'; ++j) {
                char result[65];
                int err = base_conversion(words_buffer1->words[i][j], result, 8);
                if (err) {
                    free_buffer(words_buffer1);
                    return err;
                }
                fprintf(out, "%s", result);
                putc(' ', out);
            }
        }  else {
            fprintf(out, "%s ", words_buffer1->words[i]);
        }
    }

    free_buffer(words_buffer1);
    fclose(fin1);
    fclose(out);

    return 0;
}
