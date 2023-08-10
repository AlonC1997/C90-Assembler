/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * -------------------------
 * File name: output_files.c
 * -------------------------
 * This file is responsible for:
 * - Generating the output files of the assembler.
 * - Creating the '.am', '.ob', '.ext', and '.ent' files with the assembled code and information.
 * - Writing the final assembled code, addresses, and additional data to the output files.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/error.h"
#include "headerfiles/first_and_second_pass_data.h"
#include "headerfiles/general_helpers.h"
#include "headerfiles/macro_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Functions: */
void spread_am_file_line(int to_do, FILE *process_f, macro_ptr *h, char *line, int *macro_def_flag, const char *file_name, int line_number) {
    macro_ptr temp = NULL ;
    char *p ;
    char *temp_name ;
    int count ;
    count = DEFAULT_VALUE ;

    switch (to_do) {
        case COPY_AS_IS:
            fprintf(process_f, "%s", line) ;
            return ;

        case DONT_COPY:
            return ;

        case MACRO_NAME:
            temp = *h ;
            count = consecutive_white_characters(line) ;
            p = &line[0] ;
            p += count ;
            count = consecutive_non_white_characters(p) ;
            temp_name = malloc_for_char(count + 1, file_name, line_number) ;
            strncpy(temp_name, p, count) ;
            temp_name[count] = '\0';

            while (temp != NULL) {
                if (count != strlen(macro_node_get_macro_name(temp)) || strncmp(temp_name, macro_node_get_macro_name(temp), count) != DEFAULT_VALUE) {
                    /* We are checking both conditions because we want to avoid case of substring from temp->macro_name is equal to temp_name. */
                    temp = macro_node_get_next(temp) ;
                } 
                else { /* Got first element with same macro name- supposed to be the definition line. */
                    break ;
                }
            }
            if (macro_node_get_next(temp) != NULL) {  /* The list should has next but we must check it. */
                temp = macro_node_get_next(temp) ; /* Getting first element which is line content and not definition. */
            }
            while (temp != NULL && count == strlen(macro_node_get_macro_name(temp)) && strncmp(p, macro_node_get_macro_name(temp), count) == DEFAULT_VALUE && (macro_node_get_def_flag(temp) == FALSE) && (macro_node_get_end_flag(temp) == FALSE)) {
                /* The second ^^^^^ condition is checked to avoid equalisation substring. */
                fprintf(process_f, "%s", macro_node_get_macro_content(temp)) ;
                temp = macro_node_get_next(temp) ;
            }
            *macro_def_flag = FALSE ;
            free(temp_name) ;
            return ;
    }
}

static char six_bits_to_base64_value(unsigned char value) { 
    if (value < 26) {
        return 'A' + value ;
    }
    else if (value < 52) {
        return 'a' + (value - 26) ;
    }
    else if (value < 62) {
        return '0' + (value - 52) ;
    }
    else if (value == 62) {
        return '+' ;
    }
    else { 
        return '/' ;
    }
}

void first12_bits_to_base64(short int num, FILE* output_file) {
    unsigned char arr[2] ;
    char result[3] ;

    arr[0] = (num >> 6) & 0x3F ;  /* Extract the first 6 bits of the number (always looking on 12 first bits) and store them in arr[0]. */
    arr[1] = num & 0x3F ;  /* Extract the last 6 bits of the number (always looking on 12 first bits) and store them in arr[1]. */

    result[0] = six_bits_to_base64_value(arr[0]) ; /* Convert the first 6 (always looking on 12 first bits) bits to a Base64 character and store it in result[0]. */
    result[1] = six_bits_to_base64_value(arr[1]) ; /* Convert the last 6 (always looking on 12 first bits) bits to a Base64 character and store it in result[1]. */

    result[2] = '\0';

    fprintf(output_file, "\n%s", result) ; 
    /* While getting here first time we are still on title line (first line) thats why we first going line down.
    If we will do the oposite- extra white line will print in the end and we looking for accurate file, without unnecessary white line at the end. */
}

static void print_label_on_ob(FILE *ob_f, int address, ARE type) {
    short int code ;
    short int ten_bits_address ;
    code = DEFAULT_VALUE ;
    ten_bits_address = (short int)address ;
    ten_bits_address = ten_bits_address << 2 ;
    code = (short int)type|ten_bits_address ;
    first12_bits_to_base64(code, ob_f) ;
    return ;
} 

static void print_label_on_ext(FILE *ext_f, const char *label, int using_decimal_address, int first_flag) {
    if (first_flag == TRUE) {
        fprintf(ext_f, "%s    %d", label, using_decimal_address) ;
    }
    /* first_flag to indicate if its first line of this file - helping us avoid extra white line in the end of this file.*/
    else {
        fprintf(ext_f, "\n%s    %d", label, using_decimal_address) ;
    }
    return ;
} 

void print_label_on_ent(FILE *ent_f, const char *label, int define_decimal_address, int first_flag) {
    if (first_flag == TRUE) {
        fprintf(ent_f, "%s    %d", label, define_decimal_address) ;
    }
    /* first_flag to indicate if its first line of this file - helping us avoid extra white line in the end of this file.*/
    else { /* first_flag == FALSE */
        fprintf(ent_f, "\n%s    %d", label, define_decimal_address) ;
    }
    return ;
}

int check_and_print_labels(FILE *ob_f, FILE *ext_f, int current_address, def_label_ptr *h, const char *label_name, int length, const char *file_name, int line_number, int *first_flag) {
    def_label_ptr temp = NULL;
    if (*h != NULL) {
        temp = *h;
    }
    while (temp != NULL) {
        if (length == strlen(def_label_node_get_name(temp)) && strncmp(def_label_node_get_name(temp), label_name, length) == DEFAULT_VALUE) { /* The first condition is checked to avoid equalisation substring. */
            if (def_label_node_get_extern_flag(temp)) { /* External label. */
                print_label_on_ext(ext_f, label_name, current_address, *first_flag) ;
                print_label_on_ob(ob_f, DEFAULT_VALUE, EXTERNAL) ; 
                *first_flag = FALSE ;  /* first_flag to indicate if its first line of this file - helping us avoid extra white line in the end of this file. */
                return TRUE; /* I don't need to check the whole list for possible duplicates because I checked it while adding in the first pass. */  
            }
            else { /* Defined in file. */
                print_label_on_ob(ob_f, def_label_node_get_address(temp), RELOCATABLE);
                return TRUE ; /* I don't need to check the whole list for possible duplicates because I checked it while adding in the first pass. */
            }          
        }
        temp = def_label_node_get_next(temp) ;
    }
    
    second_pass_error(NO_SUCH_LABEL_DEFINED, file_name, line_number) ;
    return FALSE ; /* Label never defined. */
} 

void print_ob_title(FILE *ob_f, int ic, int dc) {
    fprintf(ob_f, "%d %d", ic, dc) ;
    return ;
}

void print_data(FILE *ob_f, data_image_ptr *head, int address) {
    data_image_ptr temp = NULL;
    int temp_flag ;
    temp_flag = DEFAULT_VALUE ;
    if (*head != NULL) {
        temp = *head ;
    }
    while (temp != NULL) {
        if (temp_flag == 2) { /* The first we count is open char, second is last char. */
            break ;
        }
        if (data_image_node_get_address(temp) == address) {
            first12_bits_to_base64(data_image_node_get_num(temp), ob_f) ;
            if (data_image_node_get_flag(temp) == TRUE) {
                temp_flag++ ;
            }
        }
        temp = data_image_node_get_next(temp) ;
    }
}
