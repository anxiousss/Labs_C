#include <stdio.h>
#include <ctype.h>
#include <string.h>
#pragma warning(disable : 4996)

char voicelesses[] = "bcdfghjklmnpqrstvwxyz";

int is_voiceless(char c) {
    c = tolower(c);
    
    for (int i = 0; i < 21; i++)
        if (c == voicelesses[i])
            return 1;
            
    return 0;
}

int is_voiceless_word(char* word) {
    
    for (int i = 0; word[i] != '\0'; i++)
    
        if (isalpha(word[i]) && !is_voiceless(word[i]))
            return 0;

    return 1;
}

int main() {
    
    FILE* file = fopen("input.txt", "r");
    
    if (file == NULL) {
        printf("Не удалось открыть файл input.txt\n");
        return 1;
    }
    
    FILE* output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Не удалось открыть файл output.txt\n");
        return 1;
    }
    
    char str[1000];
    while (fgets(str, sizeof(str), file) != NULL) {
        
        char* word = strtok(str, " ,.\n\t");
        
        while (word != NULL) {
            
            if (is_voiceless_word(word)) {
                
                fprintf(output, "Слово '%s' содержит только звонкие согласные.\n", word);
                
                fclose(file);
                fclose(output);
                
                return 0;
            }
            
            word = strtok(NULL, " ,.\n\t");
        }
    }
    
    fprintf(output, "В файле нет слов, содержащих только звонкие согласные.\n");
    
    fclose(file);
    fclose(output);
    
    return 0;
}