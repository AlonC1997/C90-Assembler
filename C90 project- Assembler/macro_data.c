/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * Date of Submission: 10/08/2023
 * -------------------------
 * File name: macro_data.c
 * -------------------------
 * This file is responsible for:
 * - Defining and implementing data structures used for storing macro-related information.
 * - Managing the encapsulation and hiding of macro data details.
 * - Providing functions for accessing and manipulating macro data.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/error.h"
#include "headerfiles/general_helpers.h"
#include "headerfiles/handle_macro.h"
#include "headerfiles/macro_data.h"
#include <stdlib.h>
#include <string.h>

/* Struct:*/
/* Represents a node in a linked list used to store information about macros.*/
struct macro_node {
    int def_flag ; /* While 1 its def line, 0 is content or end. */
    int end_flag ; /* While 1 its end line, 0 is content or def. */
    char *macro_name ; /* Stores the name of the macro associated with this node. */
    char *macro_content ; /* Stores the content of line associated with this macro node. */
    macro_ptr next ; /* Pointer to next node in this linked list or NULL if its last element. */
} ; 

/* Functions: */
void free_macro_list(macro_ptr *h) {    
    macro_ptr temp = NULL ;
    while (*h) {
        temp = *h ;
        *h = (*h) -> next ;
        if (temp -> macro_content != NULL) {
            free(temp -> macro_content) ;
        }
        if (temp -> macro_name != NULL) {
            free(temp -> macro_name) ;
        }
        free(temp) ;
    }
}

macro_ptr malloc_for_macro(const char *file_name, int line_number) {
    macro_ptr h = NULL ;
    h = (macro_ptr) malloc(sizeof(struct macro_node)) ;
    if (h == NULL) {
        memory_error(MACRO_ALLOCATION_FAILED, file_name, line_number) ;
        exit(TRUE) ;
    } 
    return h ;
}

static void define_macro_values(macro_ptr *head, char * macro_content, int macro_def_flag, int macro_end_flag, const char *file_name, int line_number) {
    char *temp_content ;
    int count ;
    count = strlen(macro_content) ;
    temp_content = malloc_for_char(count+1, file_name, line_number) ;
    strncpy(temp_content, macro_content,count) ;
    temp_content[count] = '\0' ;
    (*head) -> macro_content = temp_content ;
    (*head) -> def_flag = macro_def_flag ;
    (*head) -> end_flag = macro_end_flag ;
}

void create_macro_node(macro_ptr *head, macro_ptr *tail, int macro_def_flag, int macro_end_flag, char * line_p, const char *file_name, int line_number) {
    /* When getting here  line_p is point to first non white char of the line or in case of macro defenition its point to first char of name (the one which come after 'mcro' if the line is mcro k1 its point to k, if the line is just k1 its point to k). */
    macro_ptr temp = NULL  ;
    char * temp_line ;
    char * temp_name ;
    int count ;  
    temp =  malloc_for_macro(file_name, line_number) ; /* Allocation will be checked at the function. */
    count = consecutive_non_white_characters(line_p) ;
    temp_line = malloc_for_char(count+1, file_name, line_number) ; /* Allocation will be checked at the function. */
    strncpy(temp_line,line_p,count) ; 
    temp_line[count] = '\0' ; /* And now we pad it. */

    /* Those are same for all cases, just the name is different. */   
    define_macro_values(&temp, line_p, macro_def_flag, macro_end_flag, file_name, line_number) ;
     
    /* Cases which determine the macro_name: */
    if (*head == NULL) { /* Case 1: first element of the list(must be Macro def).*/
        temp -> macro_name = temp_line ;
        temp -> next = NULL ;
        *head = temp;
        *tail = temp ; /* Update both head and tail for the first node added. */    
        return ;
    } 
    else { /* Cases 2 and 3: not first element  */
        if (macro_def_flag == FALSE || macro_end_flag == TRUE) { /* Case 2: end macro ("endmcro") or inside content: */ 
            count = strlen((*tail) -> macro_name) ;
            temp_name = malloc_for_char(count + 1, file_name, line_number) ;
            strncpy(temp_name,((*tail) -> macro_name),count) ;
            temp_name[count] = '\0' ;
            temp -> macro_name = temp_name ; /* Name of last macro item. */
            if (temp_line != NULL) {
                free(temp_line) ;
            } 
        }  
        else { /* Case 3: def macro line.  */
            temp -> macro_name = temp_line ; /* In this case temp_line is the exact macro name.*/
        }
        if (*tail != NULL) { /* Should never be null if get here. for safety.*/
            (*tail) -> next = temp ;
            temp -> next = NULL ;
            (*tail) = temp ; 
        }
        return ;
    }
}

int macro_node_get_def_flag(const macro_ptr node) {
    return node -> def_flag ;
}

int macro_node_get_end_flag(const macro_ptr node) {
    return node -> end_flag ;
}

const char *macro_node_get_macro_name(const macro_ptr node) {
    return node -> macro_name ;
}

const char *macro_node_get_macro_content(const macro_ptr node) {
    return node -> macro_content;
}

macro_ptr macro_node_get_next(const macro_ptr node) {
    return node -> next;
}
