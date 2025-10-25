#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tokens.h"

bool is_whitespace(char c) {
	return c == ' ' || c == '\n' || c == '\t';
}

bool is_special(char c) {
	return c == '(' || c == ')' || c == '<' || 
	       c == '>' || c == ';' || c == '|';
}

// adds character to word and increases capacity if needed
bool add_char_to_word(char **word, size_t *len, size_t *capacity, char c) {
    if (*len >= *capacity) {
        *capacity = (*capacity == 0) ? 8 : (*capacity) * 2;
        char *new_word = realloc(*word, *capacity);
        if (new_word == NULL) {
            return false;
        }
        *word = new_word;
    }
    (*word)[(*len)] = c;
    (*len)++;
    return true;
}

// null terminates given word and adds to output vector
void finish_word(char **word, size_t *len, size_t *capacity, vect_t *output) {
    if (*word != NULL && *len > 0) {
        add_char_to_word(word, len, capacity, '\0');
        vect_add(output, *word);
        free(*word);
        *word = NULL;
        *len = 0;
        *capacity = 0;
    }
}

vect_t* tokenize(const char *input) {
    vect_t *output = vect_new();
    if (output == NULL) {
        return NULL;
    }
    
    char *currWord = NULL;
    size_t wordSize = 0;
    size_t wordCapacity = 0;
    bool inQuotes = false;
    
    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        
        if (c == '"') {
            if (inQuotes) { // in quotes and recieved end quote -> add quote to output & end quote
                finish_word(&currWord, &wordSize, &wordCapacity, output);
                inQuotes = false;
            } else { // start of new quote -> add prev to output
                finish_word(&currWord, &wordSize, &wordCapacity, output);
                inQuotes = true;
            }
        }
        else if (inQuotes) { // build quote string
            add_char_to_word(&currWord, &wordSize, &wordCapacity, c);
        }
        else if (is_whitespace(c)) { // add prev to ouput
            finish_word(&currWord, &wordSize, &wordCapacity, output);
        }
        else if (is_special(c)) { // add prev & special char to output
            finish_word(&currWord, &wordSize, &wordCapacity, output);
            char special_token[2] = {c, '\0'};
            vect_add(output, special_token);
        }
        else { // build word-string
            add_char_to_word(&currWord, &wordSize, &wordCapacity, c);
        }
    }
    // add remaining word if any
    finish_word(&currWord, &wordSize, &wordCapacity, output);
    
    return output;
}
