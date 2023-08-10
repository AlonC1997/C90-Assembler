/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * -------------------------
 * File name: general_helpers.c
 * -------------------------
 * This file is responsible for:
 * - Defining general helper functions and macros used throughout the assembler.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/general_helpers.h"
#include "headerfiles/error.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Functions: */
int consecutive_white_characters(const char * line) {
    int spaces ;
    spaces = DEFAULT_VALUE ;
    while (isspace(line[spaces]) && (line[spaces] != '\0')) {
        ++spaces ;
    }
    return spaces ;
} 

int consecutive_non_white_characters(const char * line) {
    int count ;
    count = DEFAULT_VALUE ;
    while ((line[count] != '\0') && (!isspace(line[count])))  {
        ++count ;
    }
    return count ;
} 

int is_it_empty_line(const char * line) {
    int i ;
    i = DEFAULT_VALUE ;
    while((line[i] != '\0' && line[i] != '\n')) {
        if (!isspace(line[i])) {
            return FALSE ;
        }
        line++ ; 
    }
    return TRUE ;
}

int is_it_comment_char(const char * c) {
    int i ;
    i = DEFAULT_VALUE  ;
    if (c[i] == ';') {
        return TRUE ;
    }
    return FALSE ;
}

int is_it_function_name(const char *p, int max) { /* Using also for returning the exact index for function name. */
    int i ; 
    char function_names[NUM_OF_FUNCTIONS][FUNC_NAME_MAX_SIZE] = { "mov", "cmp", "add", "sub", "not",
    "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"} ; /* Auto pad with '\0'. */
    for (i = DEFAULT_VALUE; i < NUM_OF_FUNCTIONS; i++) {
        if ((strlen(function_names[i]) == max) && (strncmp(function_names[i], p, max) == DEFAULT_VALUE)) { /* The first condition is checked to avoid equalisation substring. */
            return i ; /* Function number. */
        }
    }
    return ILEGAL ;
}

int is_it_directive_name(const char *p, int max) { /* Using also for returning the exact index for directive name. */
    int i ;
    char directive_names[NUM_OF_DIRECTIVE][DIRECTIVE_MAX_SIZE] = {".data",".string",".extern",".entry"} ; /* Auto pad with '\0'. */
    for (i = DEFAULT_VALUE; i < NUM_OF_DIRECTIVE; i++) {
        if ((max == strlen(directive_names[i])) && (strncmp(directive_names[i], p, max) == DEFAULT_VALUE)) { /* The first condition is checked to avoid equalisation substring. */
            return i ; /* directive number. */
        }
    }
    return ILEGAL ;
}

int is_it_register_name(const char *p, int max, int *error_flags) {
    int i ;
    char registar_name[NUM_OF_REGISTAR][REGISTAR_MAX_SIZE] = {"r0","r1","r2","r3","r4","r5","r6","r7"} ; /* Auto pad with '\0'. */
    for (i = DEFAULT_VALUE; i < NUM_OF_REGISTAR; i++) {
        if ((max == 2) && (strncmp(registar_name[i], p, max) == DEFAULT_VALUE)) { /* The first condition is checked to avoid equalisation substring (2 is length of register). */
            return i ; /* Return register name. */
        }
    }
    /* We wont print here the error because ILEGAL (not register) is a positive answer when is_legal_label func is calling this function. 
    If its register operand and checking if label name has name of register we will print the error when we handle the line on first pass.*/
    return ILEGAL ;
}

char * malloc_for_char(int size, const char * file_name, int line_number) {
    char * ptr ;
    ptr = (char *) malloc(sizeof(char) * size); 
    if (ptr == NULL) {
        memory_error(CHAR_ARRAY_ALLOCATION_FAILED, file_name, line_number) ;
        exit(TRUE) ;
    }
    return ptr ;
}
