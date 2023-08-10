/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * -------------------------
 * File name: first_and_second_pass_data.c
 * -------------------------
 * This file is responsible for:
 * - Defining data structures used for storing information during the first and second passes.
 * - Managing the encapsulation and organization of data required for the passes.
 * - Providing functions to interact with the data structures and store/retrieve information.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/first_and_second_pass_data.h"
#include "headerfiles/error.h"
#include "headerfiles/general_helpers.h"
#include <stdlib.h>
#include <string.h>

/* Structs:*/
/* Represents a node in the main processing stage of the assembler. */
struct main_node { 
    LINE_INDICATION type ; /* Type of line indication (directive, instruction, comment, etc.). */
    Address_method source ; /* Addressing method of the source operand. */
    Address_method target ; /* Addressing method of the target operand. */
    int line_number ; /* Line number in the file for error reporting. */
    int start_address ; /* Decimal from 100 to 1023. */
    char *source_name ; /* NULL if there isnt. */
    char *target_name ; /* NULL if there isnt. */
    short int first_line_code ; /* First line of machine code. */
    short int second_line_code ; /* Second line of machine code (if exists). */
    short int third_line_code ; /* Third line of machine code (if exists). */
    main_ptr next ;  /* Pointer to the next main_node in the linked list. */
} ; 

/* Represents a node for defined labels in the assembler. */
struct def_label_node {
    char *def_label_name ;  /* Name of the defined label. */
    /* Four different flag to indicate varius problem like label is defined twice one in extern and one as regular and more...*/
    /* its possible that some will be TRUE and some will be False but this is the way to expose errors. */
    int extern_flag ; /* While 1 its came from '.extern' statement line, 0 its not came from '.extern' statement line  */
    int entry_flag ; /* While 1 its came from '.entry' statement line, 0 its not came from '.entry' statement line  */
    int regular_flag ; /* While 1 its came from regular label statement line, 0 its not regular label statement line.  
    While 1 its defined somewhere in file, 0 its never defined,
    if (After first pass done) some node  entry_flag is 1 and regular_flag is 0 so it ilegal. */
    int address ; /* Decimal from 100 to 1023. */
    int def_line ; /* Line where the label is defined (file line counter, not decimal cell counter). 
    Used for printing error messages when encountering an undefined label in the second pass. */
    def_label_ptr next ; /* Pointer to the next def_label_node in the linked list. */
} ; 

/* Represents a node for data in the assembler's data image. */
struct data_image_node {
    int address ; /* Decimal address value. */
    int flag ; /* Flag indicating if the node is the first and/or last in the data image. */
    short int num ; /* 16-bit number (only the first 12 bits are active; the last 4 bits are 0). */
    data_image_ptr next ; /* Pointer to the next data_image_node in the linked list. */
} ;

/* Functions: */
static void define_data_image_node_values(data_image_ptr *h, int address, int flag, short int num) {
    (*h) -> address = address ;
    (*h) -> flag = flag ;
    (*h) -> num = num ;
}

void create_data_image_node(data_image_ptr *head , data_image_ptr *tail, int address, int flag, short int num, const char *file_name, int line_number) {
    data_image_ptr temp = NULL ;
    temp = malloc_for_data_image(file_name, line_number) ; /* Allocation will be checked at the function. */
    define_data_image_node_values(&temp, address, flag, num) ;
    if (*head == NULL) { /* List is empty. */
        *head = temp ;
        *tail = temp ; /* Update both head and tail for the first node added. */
    } 
    else { /* List isn't empty. */
        (*tail) -> next = temp ;
        *tail = temp ; /* Update only the tail to the new node. */
    }
    temp -> next = NULL ;
    return ;
}

static void define_main_node_values(main_ptr *h, LINE_INDICATION type, Address_method source, Address_method target, int line_number, int start_address, const char *source_name, const char *target_name, int source_length, int target_length, const char *file_name) {
    char *source_temp  ;
    char *target_temp  ;
    /* Names can be null in case of if there isnt source/target opernad. */
    if (source_name != NULL) {
        source_temp = malloc_for_char(source_length+1, file_name, line_number) ;
        strncpy(source_temp, source_name, source_length) ;
        source_temp[source_length] = '\0' ;
    }
    else {
        source_temp = NULL ;
    }
    if (target_name != NULL) {
        target_temp = malloc_for_char(target_length+1, file_name, line_number) ;
        strncpy(target_temp, target_name, target_length) ;
        target_temp[target_length] = '\0' ;
    }
    else {
        target_temp = NULL ;
    }
    (*h) -> source_name = source_temp ;
    (*h) -> target_name = target_temp ;
    (*h) -> type = type ;
    (*h) -> source = source ;
    (*h) -> target = target ;
    (*h) -> line_number = line_number ;
    (*h) -> start_address = start_address ;
}
      
void create_main_node(main_ptr *head, main_ptr *tail, LINE_INDICATION type, Address_method source, Address_method target, int start_address, const char *source_name, const char *target_name, const char *file_name, int line_number, int source_length, int target_length) {
    main_ptr temp = NULL ;
    temp = malloc_for_main(file_name, line_number) ; /* Allocation will be checked at the function. */   
    define_main_node_values(&temp, type, source, target, line_number, start_address, source_name, target_name, source_length, target_length, file_name) ;
    if (*head == NULL) { /* List is empty. */
        *head = temp ;
        *tail = temp ; /* Update both head and tail for the first node added. */
    } 
    else { /* List isn't empty. */
        (*tail) -> next = temp ;
        *tail = temp ; /* Update only the tail to the new node. */
    }
    temp -> next = NULL ;
    return ;
}

static void define_def_label_node_values(def_label_ptr *h, const char *def_label_name, int length, int extern_flag, int entry_flag, int regular_flag, int address, int def_line, const char* file_name) {
    char *temp_name ;
    temp_name = malloc_for_char(length+1, file_name, def_line) ;
    strncpy(temp_name, def_label_name, length) ;
    temp_name[length] = '\0' ;
    (*h) -> def_label_name = temp_name ;
    (*h) -> extern_flag = extern_flag ;
    (*h) -> entry_flag = entry_flag ;
    (*h) -> regular_flag = regular_flag ;
    (*h) -> address = address ;
    (*h) -> def_line = def_line ;
}

int create_def_label_node(def_label_ptr *head, const char *def_label_name, int length, int extern_flag, int entry_flag, int regular_flag, int address, int def_line, int line_number, const char *file_name, int *error_flag) {
    def_label_ptr temp = NULL ;
    def_label_ptr new = NULL ;
    if (*head != NULL) {
        temp = *head ;
    }
    /* Those are errors/warnings option (0 or more possible errors per node): 
    reg && reg(error), extern && reg(error), ext&&ent(error), ext&&ext(warning), ent&ent (warning). */
    /* It is also possible the other way around, so check the "or" option for some cases. */
    /* Entry error: "No defintion for entry found in file" error will be exposed at second pass if exist. */
    while (temp != NULL) { 
        if (length == strlen(temp -> def_label_name) && strncmp(temp -> def_label_name, def_label_name, length) == DEFAULT_VALUE) { /* The first condition is checked to avoid equalisation substring. */
            if ((temp -> extern_flag == TRUE && regular_flag == TRUE) || (temp -> regular_flag == TRUE && extern_flag == TRUE)) { 
                first_pass_directive_error(EXTERNAL_LABEL_CANT_BE_REDEFINED_IN_A_FILE, file_name, line_number) ;
                temp -> extern_flag = TRUE ; /* First option in or statment: extern_flag update. */
                temp -> regular_flag = TRUE ; /* Second option in or statment: regular_flag update. */
                *error_flag = TRUE ;
                return FALSE ;
            }
            if ((temp -> extern_flag == TRUE && entry_flag == TRUE) || (temp -> entry_flag == TRUE && extern_flag == TRUE)) {
                first_pass_label_error(LABEL_CANT_BE_BOTH_EXTERNAL_AND_ENTRY, file_name, line_number) ;
                temp -> entry_flag = TRUE ; /* First option in or statment: entry_flag update. */
                temp -> extern_flag = TRUE ; /* Second option in or statment: extern_flag update. */
                *error_flag = TRUE ;
                return FALSE ;
            }
            if (temp -> regular_flag == TRUE && regular_flag == TRUE) {  /* Already defined... error.*/
                first_pass_label_error(LABEL_NAME_CANT_REDEFINED, file_name, line_number) ;
                *error_flag = TRUE ;
                return FALSE ;
            } 
            if (temp -> extern_flag == TRUE && extern_flag == TRUE) { /* (warning) */
                warning(EXTERN_STATEMENT_TWICE, file_name, line_number) ;
            }  
            if (temp -> entry_flag == TRUE && entry_flag == TRUE) { /* (warning) */
                warning(ENTRY_STATEMENT_TWICE, file_name, line_number) ;
            }   

            /* Not errors or warnings, just flags update for exist entry_statment label node or exist regular node that now declare at entry statment: */
            /* Doing this for second pass- will check if there is label that declare in entry statment but never defined. */
            if (temp -> entry_flag == TRUE && regular_flag == TRUE) { /* Just defined label line for label that appeared at entry statment. */
                temp -> regular_flag = TRUE ; /* def_line already defined in this case. */
                temp -> def_line = def_line ; /* Update the definition line for the address. (for .ent file). */
                temp -> address =  address ;
            }
            if (temp -> regular_flag == TRUE && entry_flag == TRUE)  {  /* Just entry line label for label that already defined statment. */
                temp -> entry_flag = TRUE ;
            }

            return TRUE ; /* Found exist node for this label name, updated flags, printed errors/warnings (if exist). */
        }
        if (temp -> next == NULL) { /* We dont want to lost the pointer to last element and already checked this node.*/
            break ;
        }
        else {
            temp = temp -> next ;
        }
    } 
    /* If we reach here we are on last element and thats it a new def_label node (this label_name never appeared before). */
    new = malloc_for_def_label(file_name, def_line) ; /* Allocation will be checked at the function. */
    define_def_label_node_values(&new, def_label_name, length, extern_flag, entry_flag, regular_flag, address, def_line, file_name) ;
    if (*head != NULL) {
        temp -> next = new ;
    }
    else { /* *head == NULL */
        *head = new ;
    }
    new -> next = NULL ;
    return TRUE ;
}

main_ptr malloc_for_main(const char * file_name, int line_number) {
    main_ptr h = NULL ;
    h = (main_ptr) malloc(sizeof(struct main_node)) ;
    if (h == NULL) {
        memory_error(MAIN_ALLOCATION_FAILED, file_name, line_number) ;
        exit(TRUE) ;
    } 
    return h ;
}

def_label_ptr malloc_for_def_label(const char * file_name, int line_number) {
    def_label_ptr h = NULL ;
    h = (def_label_ptr) malloc(sizeof(struct def_label_node)) ;
    if (h == NULL) {
        memory_error(DEF_LABEL_ALLOCATION_FAILED, file_name, line_number) ;
        exit(TRUE) ;
    } 
    return h ;
}

data_image_ptr malloc_for_data_image(const char * file_name, int line_number) {
    data_image_ptr h = NULL ;
    h = (data_image_ptr) malloc(sizeof(struct data_image_node)) ;
    if (h == NULL) {
        memory_error(DATA_IMAGE_ALLOCATION_FAILED, file_name, line_number) ;
        exit(TRUE) ;
    } 
    return h ;
}

void free_main_list(main_ptr *h) {
    main_ptr temp = NULL ;
    while (*h != NULL) {
        temp = *h ;
        *h = (*h) -> next ;
        if (temp -> source_name != NULL) {
            free(temp -> source_name) ;
        }
        if (temp -> target_name != NULL) {
            free(temp -> target_name) ;
        }
        free(temp) ;
    }
}

void free_def_label_list(def_label_ptr *h) {
    def_label_ptr temp = NULL ;
    while (*h != NULL) {
        temp = *h ;
        *h = (*h) -> next ;
        if (temp -> def_label_name != NULL) {
            free(temp -> def_label_name) ;
        }
        free(temp) ;
    }
}

void free_data_list(data_image_ptr *h) {    
    data_image_ptr temp = NULL ;
    while (*h != NULL) {
        temp = *h ;
        *h = (*h) -> next ;
        free(temp) ;
    }
}

void free_all_first_pass_lists(main_ptr *main_head, data_image_ptr *data_head, def_label_ptr *def_head) {
    if (*main_head != NULL) {
        free_main_list(main_head) ;
    }
    if (*data_head != NULL) {
        free_data_list(data_head) ;
    }
    if (*def_head != NULL) {
        free_def_label_list(def_head) ;
    }
}

/* Getter and Setter Functions for main_node: */
LINE_INDICATION main_node_get_type(const main_ptr node) {
    return node -> type ;
}

Address_method main_node_get_source(const main_ptr node) {
    return node -> source ;
}

Address_method main_node_get_target(const main_ptr node) {
    return node->target ;
}

int main_node_get_line_number(const main_ptr node) {
    return node -> line_number;
}

int main_node_get_start_address(const main_ptr node) {
    return node -> start_address ;
}

const char *main_node_get_source_name(const main_ptr node) {
    return node -> source_name ;
}

const char *main_node_get_target_name(const main_ptr node) {
    return node -> target_name ;
}

short int main_node_get_first_line_code(const main_ptr node) {
    return node -> first_line_code ;
}

void main_node_set_first_line_code(main_ptr node, short int code) {
    node -> first_line_code = code ;
}

short int main_node_get_second_line_code(const main_ptr node) {
    return node -> second_line_code ;
}

void main_node_set_second_line_code(main_ptr node, short int code) {
    node -> second_line_code = code ;
}

short int main_node_get_third_line_code(const main_ptr node) {
    return node -> third_line_code ;
}

void main_node_set_third_line_code(main_ptr node, short int code) {
    node -> third_line_code = code ;
}

main_ptr main_node_get_next(const main_ptr node) {
    return node -> next ;
}

/* Getter and Setter Functions for def_label_node: */
const char *def_label_node_get_name(def_label_ptr node) {
    return node -> def_label_name ;
}

int def_label_node_get_extern_flag(const def_label_ptr node) {
    return node -> extern_flag ;
}

int def_label_node_get_entry_flag(const def_label_ptr node) {
    return node -> entry_flag ;
}

int def_label_node_get_regular_flag(const def_label_ptr node) {
    return node -> regular_flag ; 
}

int def_label_node_get_address(const def_label_ptr node) {
    return node -> address ;
}

int def_label_node_get_def_line(const def_label_ptr node) {
    return node -> def_line ;
}

def_label_ptr def_label_node_get_next(const def_label_ptr node) {
    return node -> next ;
}

/* Getter Functions for data_image_node: */
int data_image_node_get_address(const data_image_ptr node) {
    return node -> address ;
}

int data_image_node_get_flag(const data_image_ptr node) {
    return node -> flag ;
}

short int data_image_node_get_num(const data_image_ptr node) {
    return node -> num ;
}

data_image_ptr data_image_node_get_next(const data_image_ptr node) {
    return node -> next ;
}
