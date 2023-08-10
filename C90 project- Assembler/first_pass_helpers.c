/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * -------------------------
 * File name: first_pass_helpers.c
 * -------------------------
 * This file is responsible for:
 * - Providing helper functions to assist in the first pass stage of the assembler.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/error.h"
#include "headerfiles/first_pass_helpers.h"
#include "headerfiles/general_helpers.h"
#include <ctype.h>
#include <string.h>

/* Functions: */
static int is_contain_alpha_and_digit_only(const char* p, int consecutive_chars) { /* Used only for is_legal_label func so error flag update there if return FALSE.*/
    int i ;
    for (i = DEFAULT_VALUE ; i < consecutive_chars ; i++) {
        if (isalpha(p[i]) || (p[i] >= '0'&& p[i] <= '9')) { 
            continue ;
        } 
        else { /* Not digit or alpha. */
            return FALSE ;
        }
    }
    return TRUE ;
}

/*
* For the avoidance of doubt:
* Registered in the forum that the label must not be registered under any circumstances. 
* For example: 'r1' is forbidden (of course not '@r1' either because it doesn't start with a letter).
* In addition, it is noted that the label is allowed to be a directive without the dot at the beginning.
* For example 'extern' but not '.extern'.
* Therefore, the option that a label will be called by the name of a does not exist in the test at all because for example:
* 'string' is valid for a label name and '.string' is invalid because it does not start with a letter.
*/
int is_it_legal_label(const char* p, int consecutive_chars, const char * file_name, int line_number, int *error_flags) {
    if (consecutive_chars >= LABEL_MAX_LENGTH) { /* Case 1: LABEL_MAX_LENGTH is 32 for logical reason (padding with one NULL char -'\0') --> Label max is 31- Mamman instructions. */ 
        first_pass_label_error(LABEL_MAX_LENGTH_IS_31, file_name, line_number) ;
        *error_flags = TRUE ;
        return FALSE ;
    }
    else if (!isalpha(p[0])) { /* Case 2: First char of label isnt letter.*/
        first_pass_label_error(LABEL_NAME_SHOULD_START_WITH_LETTER, file_name, line_number);
        *error_flags = TRUE ;
        return FALSE ;
    } 
    else if (!is_contain_alpha_and_digit_only(p,consecutive_chars)) { /* Case 3: Label doesnt contain only letters and digits.*/
        first_pass_label_error(LABEL_NAME_CONTAIN_ONLY_LETTERS_AND_NUMBERS, file_name, line_number) ;
        *error_flags = TRUE ;
        return FALSE ;
    }
    else if (!is_it_function_name(p, consecutive_chars)) { /* Case 4: Label cant be function name. */
        first_pass_label_error(LABEL_NAME_CANT_BE_AS_OPCODE_NAME, file_name, line_number) ;
        *error_flags = TRUE ;
        return FALSE ;
    }
    else if (is_it_register_name(p,consecutive_chars, error_flags) != ILEGAL){ /* Case 5: Label cant be register name. O means legal this function is mean 'r0'. -999 its not register.*/
        first_pass_label_error(LABEL_NAME_CANT_BE_AS_REGISTAR_NAME, file_name, line_number) ;
        *error_flags = TRUE ;
        return FALSE ;
    }
    else if (!is_it_directive_name(p, consecutive_chars)) { /* Case 5: Label cant be directive name. */
        first_pass_label_error(LABEL_NAME_CANT_BE_AS_DIRECTIVE_NAME, file_name, line_number) ;
        *error_flags = TRUE ;
        return FALSE ;
    }
    else { /* Case 6: Legal label. */
        return TRUE ;
    }      
}

int is_it_legal_number(const char* p, int length, const char *file_name, int line_number, int *error_flag, Operand_type range) {
    int i, is_negative, num ;
    i = DEFAULT_VALUE ;
    is_negative = FALSE ;
    num = DEFAULT_VALUE ;

    if (p[i] == '+' || p[i] == '-') { /* Case 1: Check for a leading '+' or '-'. */
        if (p[i] == '-') {
            is_negative = TRUE ;
        }
        i++ ;
    } 
    if (!(p[i] >= '0' && p[i] <= '9')) { /* Case 2: Check if the first character is a digit, Working also if not start with + \ - . */
        first_pass_general_error(A_NUMBER_SHOULD_APPEAR_AFTER_PLUS_OR_MINUS, file_name,line_number) ;
        *error_flag = TRUE ;
        return ILEGAL ;
    }
    while (i < length && (p[i] >= '0' && p[i] <= '9')) { /* Convert the characters to an integer */
        num = (num * 10) + (p[i] - '0') ;
        i++ ;
    }
    if (i < length) { /* Case 3: Encounter non-digit. */
        *error_flag = TRUE ; 
        first_pass_general_error(ONLY_INTEGERS_NUMBERS_ARE_ACCEPT, file_name,line_number) ;
        return ILEGAL ;  /* If we reach here, its mean iLegal number- Encounter non-digit. */
    } 
    if (is_negative) {/* Apply the minus if necessary(if its negative number). */
        num = -num ; 
    }
    if (range == REGULAR_OPERAND && (num < -512 || num > 511)) { /* Case 4: Check if the number is out of range 10 bits represent. */
        memory_error(MAX_OPERAND_INTEGER, file_name,line_number) ;
        *error_flag = TRUE ;
        return ILEGAL ;
    }
    if (range == DATA_NUM && (num < -2048 || num > 2047)) { /* Case 5: Check if the number is out of range 12 bits represent. */
        memory_error(MAX_DATA_INTEGER, file_name,line_number) ;
        *error_flag = TRUE ;
        return ILEGAL ;
    }
    return num ; /* Case 6: legal num.*/
}

int count_until_next_operand(const char *p, int *error_flag, const char* file_name, int line_number, Operand_type type) { /* Assume: When using this function p is white chars before comma or on the comma itself. */
    int commas, white, char_count;  
    char_count = DEFAULT_VALUE ;
    white = DEFAULT_VALUE ;
    commas = DEFAULT_VALUE ;
    while (*p != '\0') { /* Parsing. */
        if (!isspace(*p)) { /* Case 1: Not white. */
            if (*p == ',') { /* Case 1.1: Encouter comma. */
                commas++ ;
                if (commas > 1) { /* Case 1.1.1: Two consecutive commas. */
                    *error_flag = TRUE ;
                    first_pass_general_error(TWO_CONSECUTIVE_COMMAS, file_name, line_number) ;
                    return ILEGAL ;
                }
            }
            else { /* Case 1.2: Non white and not comma. */
                if (commas != 1) { /* Case 1.2.1:  Missing comma. */
                    *error_flag = TRUE ;
                    first_pass_general_error(MISSING_COMMA, file_name, line_number) ;
                    return ILEGAL ;
                }
                else { /* Case 1.2.2: Legal, commas = 1 */
                    return char_count ;
                } 
            }
            char_count++ ;
            p++ ;
        } 
        else { /* Case 2: Its white char. */
            white++ ;
            char_count++ ;
            p++ ;
        }          
    }
    /* Case 2: If we reach here- Ilegal. Printing appropiate error. */
    /* (*p == '\0')  Can reach here only after one non white char,
    Because before calling this function we checked if the whole reaming chars are white and got NO.
    So its must being a missing parameter error or ilegal comma at the end of data/entry/ */ 
    if (type == DATA_NUM) { 
        first_pass_directive_error(MISSING_NUM_AFTER_COMMA_OR_ILEGAL_COMMA_AT_END, file_name, line_number) ;
    }   
    else if (type == LABEL_OPERAND) { /* Using for Extern statment that not empty after first label (checked before calling) but . */
        first_pass_directive_error(MISSING_LABEL_AFTER_COMMA_OR_ILEGAL_COMMA_AT_END, file_name, line_number) ;
    }     
    else { /* (type == ELSE) Checking for general operand can be used for address method 1/3/5.. */
        first_pass_general_error(MISSING_TARGET_PARAMTER, file_name, line_number) ;
    } 
    *error_flag = TRUE ;
    return ILEGAL ;
}

CHARS_INDICATE check_next_non_white_char(const char* p) {
    while (*p != '\0') {
        if (!isspace(*p)) {
            if (isdigit(*p)) {
                return NUM ;
            } 
            else if (*p == '+' || *p == '-') {
                return MATH_SIGN ;
            } 
            else if (isalpha(*p)) {
                return LETTER ;
            } 
            else if (*p == '@') {
                return ATSIGN ;
            } 
            else if (*p == ',') {
                return COMMA ;
            } 
            else {
                return NONE ;
            }
        }
        p++;
    }
    return NULL_CHAR;
}

int count_consecutive_non_white_or_until_first_comma(const char* p) {
    int count ;
    count = DEFAULT_VALUE ;
    while (*p != '\0' && *p != ',' && !isspace(*p)) {
        count++;
        p++;
    }
    return count;
}

LINE_INDICATION line_indication(const char *p, int consecutive_chars,  int *error_flag, const char* file_name, int line_number) {
    int i ;
    char* possible_names[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne",
    "red", "prn", "jsr", "rts", "stop", ".entry", ".extern", ".string", ".data"} ;
    
    for (i = DEFAULT_VALUE ; i <= DATA ; i++) { 
        if (consecutive_chars == strlen(possible_names[i]) && strncmp(p, possible_names[i], consecutive_chars) == DEFAULT_VALUE) { /* The first condition is checked to avoid equalisation substring. */
            return i ;
        }
    }
    if (consecutive_chars >= 2) { /* Case 1: Checking this first to avoid segment fault. */
        if (p[DEFAULT_VALUE] == '.' && p[1] == '.' ) { /* Case 1.1: Two consecutive dot. */
            *error_flag =  TRUE ;
            first_pass_directive_error(TWO_CONSECUTIVE_DOT, file_name, line_number) ;
            return ILEGAL ;
        }
    }
    if (p[consecutive_chars-1] == ':') { /* Case 2: Label indication. */ 
        if (p[consecutive_chars-2] == ':') { /* Case 2.1: Two consecutive colons. */
            *error_flag =  TRUE ;
            first_pass_label_error(TWO_CONSECUTIVE_COLON, file_name, line_number) ;
            return ILEGAL ;
        }
        else if (!is_it_legal_label(p, consecutive_chars-1 ,file_name,line_number,error_flag)) { /* Case 2.2: Ilegal label. */
            return ILEGAL ; /* Error already printed, and flag was updated. */
        }
        else { /* Case 2.3: Legal label.*/
            return LABEL ; 
        }
    }
    else { /* Case 3: Not a language word, not a comment line (Was checked before calling this function.), not a colon at the end, so must be an invalid label. */
        is_it_legal_label(p, consecutive_chars ,file_name,line_number,error_flag) ; /* This function doesnt check for colon because label can be a parameter (without colon). Sending to print maximum errors.*/
        *error_flag =  TRUE ;
        first_pass_label_error(MISSING_COLON, file_name, line_number) ; /* For sure missing colon because of the last 'if' we checked not True.*/
        return ILEGAL ;
    }
    /* Case 4: Should never reach here: */
    return ILEGAL ;
}
