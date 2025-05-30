#include "solution.h"

int len(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        ++i;
    }
    return i;
}

int my_toupper(int c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    }
    return c;
}

int find_min_base(const char *number) {
    int max_digit = 0;
    char c;

    for (int i = 0; number[i] != '\0'; i++) {
        c = number[i];

        if (c >= '0' && c <= '9') {
            int digit = c - '0';
            if (digit > max_digit) {
                max_digit = digit;
            }
        }
        else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            int digit = my_toupper(c) - 'A' + 10;
            if (digit > max_digit) {
                max_digit = digit;
            }
        }
    }

    return max_digit + 1;
}

int convert_to_decimal(const char *number, int base, char* convert_number) {
    int length = len(number);
    long long decimal = 0;
    int digit;
    int sign = 1;
    int i = 0;

    if (number[0] == '-') {
        sign = -1;
        i++;
    }

    while (i < length && number[i] == '0') {
        i++;
    }

    for (; i < length; i++) {
        char c = number[i];
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 10;
        } else {
            return Invalid_input;
        }

        if (digit >= base) {
            return Invalid_input;
        }

        decimal = decimal * base + digit;
    }

    decimal *= sign;

    sprintf(convert_number, "%lld", decimal);
    return 0;
}

void remove_leading_zeros(const char* number, char* result) {
    int length = len(number);
    int i = 0;
    int is_negative = 0;

    if (number[0] == '-') {
        is_negative = 1;
        i++;
    }
    while (i < length && number[i] == '0') {
        i++;
    }

    if (i == length) {
        if (is_negative) {
            strcpy(result, "-0");
        } else {
            strcpy(result, "0");
        }
        return;
    }

    if (is_negative) {
        result[0] = '-';
        strcpy(result + 1, number + i);
    } else {
        strcpy(result, number + i);
    }}

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

int split_by_words(WordsBuffer* buffer, FILE* file, int* size) {
    if (buffer == NULL || file == NULL)
        return Memory_leak;

    char c, last_c = 0;
    int err;
    while ((c = fgetc(file)) != EOF) {
        if ((c == ' ' || c == '\n' || c == '\t') && (last_c != ' ' && last_c != '\n' && last_c != '\t')) {
            if (buffer->size > 0) {
                err = insert_symbol(buffer, '\0');
                if (err == Memory_leak)
                    return err;

            }
            err = insert_word(buffer);
            if (err == Memory_leak)
                return err;
        } else if(c != ' ' && c != '\n' && c != '\t') {
            if (buffer->size == 0) {
                err = insert_word(buffer);
                if (err == Memory_leak)
                    return err;
            }
            err = insert_symbol(buffer, c);
            if (err == Memory_leak)
                return err;
        }
        last_c = c;
    }

    if (last_c >= ' ') {
        *size = buffer->size;
    } else {
        *size = buffer->size - 1;
    }


    if (buffer->size > 0) {
        insert_symbol(buffer, '\0');
    }

    return 0;
}

int solution(FILE** in, FILE** out) {
    WordsBuffer* wordsBuffer = create_buffer();
    if (wordsBuffer == NULL) {
        return Memory_leak;
    }
    int border;
    int res1 = split_by_words(wordsBuffer, *in, &border);
    if (res1 != 0){
        free_buffer(wordsBuffer);
        return Memory_leak;
    }

    // logic
    char* number;
    char* convert_number;
    int min_base;

    for (int i = 0; i < border; ++i) {
        number = (char*) malloc(sizeof(char) * (wordsBuffer->word_sizes[i] + 1));
        if (number == NULL) {
            free_buffer(wordsBuffer);
            return Memory_leak;
        }

        convert_number = (char*) malloc(sizeof(char) * 65);
        if (convert_number == NULL) {
            free(number);
            free_buffer(wordsBuffer);
            return Memory_leak;
        }
        remove_leading_zeros(wordsBuffer->words[i], number);
        min_base = find_min_base(number);
        if (strcmp(number, "") == 0)
            break;

        int tmp = convert_to_decimal(number, min_base, convert_number);
        if (tmp) {
            free(number);
            free(convert_number);
            free_buffer(wordsBuffer);
            return tmp;
        }
        fprintf(*out, "number: %s base: %d decimal %s\n", number, min_base, convert_number);
        free(number);
        free(convert_number);
    }
    free_buffer(wordsBuffer);
    fclose(*in);
    fclose(*out);
    return 0;
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
        return 1;
    }
    return 0;
}