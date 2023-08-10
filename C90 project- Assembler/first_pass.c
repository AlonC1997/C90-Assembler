/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * -------------------------
 * File name: first_pass.c
 * -------------------------
 * This file is responsible for:
 * - Implementing the first pass of the assembler process.
 * - Handling label definitions, directives, and opcode parsing.
 * - Collecting and organizing data required for the first pass.
 * - Preparing the assembler data structures for accurate processing.
 * - Validating instructions and operands based on predefined rules.
 * -------------------------
 * Instructions from the forum:
 * - If the line is empty after an external or an entry, it is an error.
 * - On the other hand, if this happens after a data or string, it's a warning.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/error.h"
#include "headerfiles/general_helpers.h"
#include "headerfiles/second_pass.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Functions: */
static short int operands_line_code(short int opcode, short int source_method, short int target_method) {
    short int code ;
    code = DEFAULT_VALUE ;
    opcode = opcode << 5 ; 
    source_method = source_method << 9 ;
    target_method = target_method << 2 ; 
    code = opcode|source_method|target_method ; /* 4 last bits [12-15] will be zero. because we need only first 12 bits.*/
    return code ;
}

static short int register_operands(short int source_registar, short int target_registar) {
    short int code ;
    code = DEFAULT_VALUE ;
    source_registar = source_registar << 7 ;
    target_registar = target_registar << 2 ;
    code = source_registar|target_registar ;
    return code ;
} 

static int is_it_exist_file_defined_label(const char *p, int consecutive_chars, def_label_ptr *h) {
    def_label_ptr temp = NULL ;
    temp = *h ;
    while (temp != NULL) {
        if (consecutive_chars == strlen(def_label_node_get_name(temp)) && strncmp(def_label_node_get_name(temp), p, consecutive_chars) == DEFAULT_VALUE) { /* The first condition is checked to avoid equalisation substring. */
            if (def_label_node_get_regular_flag(temp) == TRUE) {
                return TRUE ; /* Label already defined at file. */
            }
            return FALSE ;
        }
        temp = def_label_node_get_next(temp) ;
    }
    return FALSE ;
}

/* Full documentation in Header file. */
/* This function is an algorithm for line with '$label_name:$' start. There are some possible errors. */
static int handle_def_label_line(const char *p, int length, int *error_flag, const char* file_name, int line_number, def_label_ptr *head, int address) { /* Using this function after check if 'name_of_label:' statment is valid. */
    const char *temp_p ;
    LINE_INDICATION after_label ; 
    int count ;
    int count_char ;
    count = DEFAULT_VALUE ;
    count_char = DEFAULT_VALUE ;
    after_label = DEFAULT_VALUE ;
    temp_p = p ; /* p is on the first char in the line. (first letter of label name). */
    count_char += length ; 
     
    /* Length is include ':' char in the end. */
    if (is_it_exist_file_defined_label(p, length-1, head)) {/* Case 1: Sending to check if exist (without ':'). The calling function will also verify its not entry or extern label. */
       *error_flag = TRUE ;
        first_pass_label_error(LABEL_NAME_CANT_REDEFINED, file_name, line_number) ;
        return ILEGAL ;
    } 
    temp_p += length ; /* temp_p is on first char after ':' .*/
    if (is_it_empty_line(temp_p))  { /* Case 2: empty after legal label name. */
        warning(LABEL_DEFINED_NOTHING, file_name, line_number) ;
        return ILEGAL ; /* Not really ilegal, using this for indicate also to get next line. nothing changed in this file nothing will be save. */
    }
    if(check_next_non_white_char(temp_p) == COMMA) { /* Case 3: Ilegal comma. */
        first_pass_general_error(ILEGAL_COMMA, file_name, line_number) ;
        *error_flag = TRUE ;
        return ILEGAL ; 
    }
    count = consecutive_white_characters(temp_p) ;
    count_char += count ;
    temp_p += count ; /* temp_p is on the first non white char after 'name_of_label:' sequence. */
    count = consecutive_non_white_characters(temp_p) ; /* Counting second non white chars sequence in the line. */
    after_label = line_indication(temp_p, count, error_flag, file_name, line_number) ;
    if (after_label == LABEL) { /* Case 4: label after label. */
        *error_flag = TRUE ;
        first_pass_label_error(LABEL_CANT_DEFINED_LABEL, file_name, line_number) ;
        return ILEGAL ;
    }
    else if (after_label == ILEGAL) { /* Case 5: Ilegal statment after label name. */
        return ILEGAL ; /* Flag was updated and error printed. */
    }
    /* For last cases we will return count_char which is help us increment p in the calling function to the first char of (entry/extern/string/data/opcode) statment so we will contiune handle the line. */
    else if (after_label == ENTRY) { /* Case 6: Label defined '.entry' warning will be printed and ignoring the label. */
        warning(LABEL_BEFORE_ENTRY, file_name, line_number) ; /* Mamman instructions: Ignore the label before entry statment, handle the entry sentence normally without saving the label. */
        return count_char ; 
    }
    else if (after_label == EXTERN) { /* Case 7: Label defined '.extern' warning will be printed and ignoring the label. */
        warning(LABEL_BEFORE_EXTERN, file_name, line_number) ; /* Mamman instructions: Ignore the label before extern statment, handle the extern sentence normally without saving the label. */
        return count_char ; 
    }
    else { /* Case 8: Got one of the line indication but not label/extern/entry. Its (opcode or data or string) Adding label to the list and, sending length till first char of opcode/data/string statment so we  we will contiune handle the line. */
        if (create_def_label_node(head, p, length-1, FALSE, FALSE, TRUE, address, line_number, line_number, file_name, error_flag)) { /* Case 8.1: Name not include ':' at the end of label name so we send length-1. */
            return count_char ;
        }
        else { /* Case 8.2: create_def_label_node indicate error. Flag was update, error was print. */
            return ILEGAL ;
        }
    }
}

/* Full documentation in Header file. */
/* This function is an algorithm for line with '.data' start. There are some possible errors. */
static int handle_data_line(const char *p, int *error_flag, const char* file_name, int line_number, main_ptr *main_head, main_ptr *main_tail, data_image_ptr *data_head, data_image_ptr *data_tail, short int address) { /* Using this function after check if '.data' statment is valid. */
    int count, first, num, i ;
    num = DEFAULT_VALUE ;
    first = TRUE ;
    /* p is on the very first char after 'a' in '.data'.*/
    i = DEFAULT_VALUE ; /* Counter to help increase dc. Will be return if the line is legal. */
    count = check_next_non_white_char(p) ;
    if  (count == NULL_CHAR) { /* Case 1: Empty after '.data' statment. */
        warning(DATA_IS_EMPTY, file_name, line_number) ; /* Just warning, its legal. */
        return DEFAULT_VALUE ;
    }
    if (count != NUM && count != MATH_SIGN ) { /* Case 2: Ilegal start. */
        if (count == COMMA) { /* Case 2.1: Ilegal comma. */
            first_pass_general_error(ILEGAL_COMMA, file_name, line_number) ;
        }
        else { /* Case 2.2: Unapropiate operand for data. */
            first_pass_directive_error(DATA_LINE_OPERANDS_ARE_INTEGER, file_name, line_number) ;
        }
        *error_flag = TRUE ;  
        return FALSE ;
    }
    else { /* Case 3: Legal start, send to check all. */
        count = consecutive_white_characters(p) ;
        p += count ; /* Standing on the very first non white after '.data' statment. */
        while (*p != '\0') {
            count = count_consecutive_non_white_or_until_first_comma(p) ;
            num  = is_it_legal_number(p, count, file_name, line_number, error_flag, DATA_NUM) ;
            if (num == ILEGAL) { /* Case 3.1: Ilegal operand. */
                return FALSE ;
            }
            else { /* Case 3.2: Num is legal. */
                p += count ; /* p is on first char after num (could be anything). */
                if (is_it_empty_line(p)) {  /* Case 3.2.1: Empty after num- legal.  */
                    create_data_image_node(data_head, data_tail, address, TRUE,(short int)num, file_name, line_number) ; /* Last item flag is true. */
                    create_main_node(main_head, main_tail, DATA, NONE_ADDRESS, NONE_ADDRESS, address, NULL, NULL, file_name, line_number, i+1, DEFAULT_VALUE) ;
                    return i+1  ;
                }
                count = count_until_next_operand(p, error_flag, file_name, line_number, DATA_NUM) ;
                if (count == ILEGAL) { /* Case 3.2.2: Found error between both parameters. */
                    return FALSE ;
                }
                /* Case 3.2.3: Legal parameters, keep parsing. */
                create_data_image_node(data_head, data_tail, address, first,(short int)num, file_name, line_number) ; /* first item flag is true. */
                first = FALSE ; 
                p += count ; /* p is on the first non comma or white after parameter. */
                i++ ;
            }  
        }
    }
    return FALSE ; /* Case 4: Will never reach here.*/
}  

/* Full documentation in Header file. */
/* This function is an algorithm for line with '.string' start. There are some possible errors. */
static int handle_string_line(const char *p, int *error_flag, const char* file_name, int line_number, main_ptr *main_head, main_ptr *main_tail, data_image_ptr *data_head, data_image_ptr *data_tail, short int address) { /* Using this function after check if '.string' statment is valid. */
    int i, first ;
    i = DEFAULT_VALUE ;
    first = TRUE ;
    /* p is on the very first char after 'g'  in '.string'.*/
    if (is_it_empty_line(p)) { /* Case 1: empty after string. Warnning. */ 
        warning(STRING_IS_EMPTY, file_name, line_number) ;
        return FALSE ; /* Not really ilegal just indicate */
    }
    i = consecutive_white_characters(p) ;
    if (p[i] == p[i+1] && p[i] == '"') { /* Case 2: Not empty because of NULL character, length will be 1. */
        create_data_image_node(data_head, data_tail, address, TRUE,(short int)DEFAULT_VALUE, file_name, line_number) ; /* first and also last (NULL) item flag is true. */
        create_main_node(main_head, main_tail, STRING, NONE_ADDRESS, NONE_ADDRESS, address, NULL, NULL, file_name, line_number, 1, DEFAULT_VALUE) ;
        return 1 ;
    }
    if (p[i] != '"') { /* Case 3: string should start with " char. Only regular keyboard " are accept. */
        first_pass_directive_error(STRING_SHOULD_START_WITH_PAIRS_OF_QUOTATION_MARKS, file_name, line_number) ; 
        *error_flag = TRUE ;
        return FALSE ; 
    }
    else { /* Case 4: Start with " */
        p+=i ;
        p++ ; /* p is on first char after first " char. */
        i = DEFAULT_VALUE ; /* From now i is counter. */
        while (*p != '\0') { /* While didnt reach null char yet.*/
            if (*p != '"') { /* Case 4.1: Didnt close the string yet. creating data node. */
                create_data_image_node(data_head, data_tail, address, first,(short int)p[0], file_name, line_number) ; /* first item flag is true. */
                i++ ;
                p++ ;
            }
            else { /* Case 4.2: Reach close " char. */
                p++ ; /* p is on first char after second " char. */
                if (!is_it_empty_line(p)) { /* Case 4.2.1: Ilegal string. */
                    first_pass_directive_error(STRING_DIRECTIVE_CONTAIN_ONE_STRING_FOLLOWED_BY_NOTHING, file_name, line_number) ;
                    *error_flag = TRUE ;
                    return FALSE ; /* Not really ilegal just indicate */
                }
                else { /* Case 4.2.2: Legal string. */
                    create_data_image_node(data_head, data_tail, address, TRUE ,(short int)'\0' ,file_name, line_number) ; /* last item flag is true.  Adding '\0' asci value to list. */
                    create_main_node(main_head, main_tail, STRING, NONE_ADDRESS, NONE_ADDRESS, address, NULL, NULL, file_name, line_number, i+1, DEFAULT_VALUE) ;
                    return i+1 ; /* return length +1 for null char. */
                }
            }
            first = FALSE ; /* From now all chars are not start, the end char (NULL) flag will be true. */
        }
        /* Reaching end of line but didnt encounter closing quotation marks. */
        first_pass_directive_error(STRING_SHOULD_END_WITH_PAIRS_OF_QUOTATION_MARKS, file_name, line_number) ; 
        *error_flag = TRUE ;
        return FALSE ; 
    }
    return FALSE ; /* Will never reach here.*/
}

/* Full documentation in Header file. */
/* This function is an algorithm for line with '.entry' start. There are some possible errors. */
static void handle_entry_line (const char *p, int *error_flag, const char* file_name, int line_number, def_label_ptr *h, int address) { /* Will be called after check if '.entry' start statment is legal. */
    int count ;
    count = DEFAULT_VALUE ;
    if (is_it_empty_line(p)) { /* Case 1: p is on the very first char after 'y' in '.entry'.*/
        first_pass_directive_error(ENTRY_IS_EMPTY, file_name, line_number) ; 
        *error_flag = TRUE ;
        return ;
    } 
    else if(check_next_non_white_char(p) == COMMA) { /* Case 2: */
        first_pass_general_error(ILEGAL_COMMA, file_name, line_number) ;
        *error_flag = TRUE ;
        return ; 
    }
    else { /* Case 3: Not empty. */
        count = consecutive_white_characters(p) ; 
        p += count ; /* p is on the very first non white char after entry statment.*/

        /* An entry line contains only one label, however, if the label is valid and there is a comma after it, I print that there is ADDITIONAL_UNRELATED_TEXT instead of saying that "label_name," is invalid. */
        count = count_consecutive_non_white_or_until_first_comma(p) ; /* Reason for checking untill comma/white because ^^.*/

        if (!is_it_legal_label(p, count, file_name, line_number, error_flag)) { /* Case 3.1: This function doesnt check if the label is exist because its also good for label parameter which come sometimes before defination.*/
            /* If we are here its ilegal, Flag already changed and error was printed. */
            return ;
        }
        else if (!is_it_empty_line(p+count)) { /* Case 3.2: The label is legal. We must check if there is nothing after it because entry statment include one label only. */
            first_pass_directive_error(ENTRY_CONTAIN_ONE_LABEL_FOLLOWED_BY_NOTHING, file_name, line_number) ;
            *error_flag = TRUE ;
            return ;
        }
        else { /* Case 3.3: line is legal. */
            if (!create_def_label_node(h, p, count, FALSE, TRUE, FALSE, address, line_number, line_number, file_name, error_flag)) {
                return ; /* If we are here its ilegal, Flag already changed and error was printed. */
            }
            else {  /* Case 3.3.2 : LEGAL. All data nodes were created. */
                return ;
            }
        }
    }
}

/* Full documentation in Header file. */
/* This function is an algorithm for line with '.extern' start. There are some possible errors. */
static void handle_extern_line (const char *p, int *error_flag, const char* file_name, int line_number, def_label_ptr *h, int address) { /* Will be called after check if '.extern' start statment is legal. */
    int count ;
    count = DEFAULT_VALUE ;
    if (is_it_empty_line(p)) { /* Case 1: p is on the very first char after 'n' in '.extern'.*/
        first_pass_directive_error(EXTERN_IS_EMPTY, file_name, line_number) ; 
        *error_flag = TRUE ;
        return ;
    } 
    else if(check_next_non_white_char(p) == COMMA) { /* Case 2: Ilegal comma. */
        first_pass_general_error(ILEGAL_COMMA, file_name, line_number) ;
        *error_flag = TRUE ;
        return ; 
    }
    else { /* Case 3: Not empty after '.extern' starting parse. */
        count = consecutive_white_characters(p) ;
        p += count ; /* Standing on the very first non white char after '.extern' statment. */
        while (*p != '\0') { 
            count = count_consecutive_non_white_or_until_first_comma(p) ; /* Counting consecutive characters untill first white char or first comma. */
            if (!is_it_legal_label(p, count, file_name, line_number, error_flag)) { /* Case 3.1: This function doesnt check if the label is exist because its also good for label parameter which come sometimes before defination.*/
                /* If we are here its ilegal, Flag already changed and error was printed. */
                return ;
            }
            else { /* Case 3.2: Label is legal. */
                if (!create_def_label_node(h, p, count, TRUE, FALSE, FALSE, address,  line_number, line_number, file_name, error_flag)) { /* Case 3.2.1: Encounter error while parsing the label name. */
                    return ; /* If we are here its ilegal, Flag already changed and error was printed. */
                }
                else if (is_it_empty_line(p+count)) { /* Case 3.2.2: Legal- nothing after legal label. Labels node created in if statment ^^^ . */
                    return ;
                }
                else { /* Case 3.2.3: Not empty after label, should come one comma and another label. Sending to count and check*/
                    p += count ; /* p is on first char after first label can be white or comma. */
                    count = count_until_next_operand(p, error_flag, file_name, line_number, LABEL_OPERAND) ;
                    if (count != ILEGAL) { /* Case 3.2.3.1: Legal chars sequence between operands.*/
                        p += count ; /* p is on first char of possible next label name.*/
                        continue ;
                    }
                    else { /* Case 3.2.3.2: Ilegal, Errors was printed and flag already updated. */
                        return ;
                    }   
                }        
            }
        }
    }
    return ; /* Should never reach here. */
}

/* Full documentation in Header file. */
/* This function is an algorithm for line with opcode and no operands. */
/* Handles the processing of / 'rts'  / 'stop'  opcodes line during the first pass of the assembler. */
static int handle_opcode_without_operands(LINE_INDICATION type, main_ptr *main_head, main_ptr *main_tail, const char* p, int start_address, const char *file_name, int line_number, int* error_flag) {
    if (!is_it_empty_line(p)) { /* Case 1: Standing on the very first char after opcode word (White or Null for sure, otherwise didn't get this function). */
        first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ;
        *error_flag = TRUE ;
        return FALSE ;
    } 
    else { /* Case 2: Legal line. */
        create_main_node(main_head, main_tail, type, NONE_ADDRESS, NONE_ADDRESS, start_address, NULL, NULL, file_name, line_number, DEFAULT_VALUE, DEFAULT_VALUE) ;
        main_node_set_first_line_code(*main_tail, operands_line_code(type, NONE_ADDRESS, NONE_ADDRESS)) ;
        return 1 ; /* One memory cell. */
    }
}

/* Full documentation in Header file. */
/* This function is an algorithm for line with opcode and one operands. There is a lot of possible different errors. */
/* Handles the processing of a 'prn' (A small change in the target operand) / 'jsr' / 'red'  / 'bne'  / 'jmp'  / 'dec'  / 'inc'  / 'not'  / 'clr'  opcodes line during the first pass of the assembler. */
static int handle_opcode_with_one_operand(LINE_INDICATION type, main_ptr *main_head, main_ptr *main_tail, const char* p, int start_address, const char *file_name, int line_number, int* error_flag) {
    int count, first, num ;
    count = DEFAULT_VALUE ;
    first = DEFAULT_VALUE ;
    num = DEFAULT_VALUE ;

    if (is_it_empty_line(p)) {/* Case 1: Standing on the very first char after opcode word (White one for sure, otherwise didnt get this function).  */
        first_pass_general_error(MISSING_TARGET_PARAMTER, file_name, line_number) ; 
        *error_flag = TRUE ;
        return FALSE ;
    } 
    
    /* Im going to check for each if, if line is empty after parameter and not just once before all if,
    because i want to analyze if the parameter before is legal and if not this was the first error to print. */

    else { /* Case 2: Not empty. */
        count = consecutive_white_characters(p) ;
        p += count ; /* Standing on the very first non white char after 'prn' . */
        first = check_next_non_white_char(p) ;
        count = consecutive_non_white_characters(p) ; /* Length of operand. */
        if (first == MATH_SIGN || first == NUM) { /* Case 2.1: Num checking. */
            if (type != PRN) { /* Case 2.1.1: Only 'prn' can get num as target operand in this family. */
                first_pass_general_error(UNAPROPIATE_TARGET_PARAMETER, file_name, line_number) ; 
                *error_flag = TRUE ;
                return FALSE ;
            }
            num = is_it_legal_number(p, count, file_name, line_number, error_flag, REGULAR_OPERAND) ;
            if (num != ILEGAL) { /* Case 2.1.2: Num is legal. */
                if (is_it_empty_line(p+count)) {  /* Case 2.1.2.1: Legal. Nothing after. Update main list. */
                    create_main_node(main_head, main_tail, type, NONE_ADDRESS, IMMEDIATELY_ADDRESS, start_address, NULL, NULL, file_name, line_number, DEFAULT_VALUE , DEFAULT_VALUE) ;
                    main_node_set_first_line_code(*main_tail, operands_line_code(type, NONE_ADDRESS, IMMEDIATELY_ADDRESS)) ;
                    main_node_set_second_line_code(*main_tail, num << 2) ; /* Num is 10 bits. */
                    return 2 ; /* Two memory cell. */
                }
                else { /* Case 2.1.2.2: Extra text- Ilegal. */
                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ;
                    return FALSE ;
                }
            }
            else { /* Case 2.1.3: Ilegal num, errors printed, flag was update.*/
                return FALSE ;
            }
        }
        else if (first == ATSIGN) { /* Case 2.2: @ registar checking. */
            p++; /* Sould standing on 'r' (after '@'). */
            if (isspace(p[0])) { /* Case 2.2.1: empty after '@'. */
                first_pass_general_error(NAME_OF_REGISTAR_IS_MISSING_AFTER_ATSIGN, file_name, line_number) ; 
                *error_flag = TRUE ;
                return FALSE ;
            }
            num = is_it_register_name(p, count-1 , error_flag) ; /* count -1 because we already increment p to 'r', was included '@' before. */ 
            if (num != ILEGAL) { /* Case 2.2.2: Legal register name. */
                if (is_it_empty_line(p+count-1)) {  /* Case 2.2.2.1: Legal. Update main list. */
                    create_main_node(main_head, main_tail, type, NONE_ADDRESS, DIRECT_REGISTER_ADDRESS, start_address, NULL, NULL, file_name, line_number, DEFAULT_VALUE , DEFAULT_VALUE) ;
                    main_node_set_first_line_code(*main_tail, operands_line_code(type, NONE_ADDRESS, DIRECT_REGISTER_ADDRESS)) ;
                    main_node_set_second_line_code(*main_tail, (short int)num << 2) ; /* Register will be between second bit to sixth bits. */
                    return 2 ; /* Two memory cell. */
                }            
                else { /* Case 2.2.2.2: Ilegal register name. */
                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ;
                    return FALSE ;
                }
            }
            else { /* Case 2.2.3: Ilegal register name. error flag update inside the calling fucntion.  */
                first_pass_general_error(ILEGAL_REGISTER_NAME, file_name, line_number) ; /* Not printed inside the calling function because of logical considerations. */
                return FALSE ;
            }
        }
        else if (first == LETTER){ /* Case 2.3: Label checking. */
            num = is_it_legal_label(p, count, file_name, line_number, error_flag) ;
            if (num != FALSE) { /* Case 2.3.1: Legal label. Update main lists. */
                create_main_node(main_head, main_tail, type, NONE_ADDRESS, DIRECT_ADDRESS, start_address, NULL, p, file_name, line_number, DEFAULT_VALUE , count) ;
                main_node_set_first_line_code(*main_tail, operands_line_code(type, NONE_ADDRESS, DIRECT_ADDRESS)) ;
                return 2 ; /* Two memory cell. */
            }
            else { /* Case 2.3.2: Ilegal label name, errors printed, flag was update.*/
                return FALSE ;
            }
        }
        else { /* Case 2.4: */
            first_pass_general_error(UNAPROPIATE_TARGET_PARAMETER, file_name, line_number) ; 
            *error_flag = TRUE ;
            return FALSE ;
        }
    }
    return FALSE ; /* Case 3: Should never reach here. */   
}

/* Full documentation in Header file. */
/* This function is an algorithm for line with opcode and two operands. There is a lot of possible different errors. */
/* Handles the processing of a 'cmp'(A small change in the target operand) / 'mov' / 'add'  / 'sub' / 'lea' (source is only direct label address) opcodes line during the first pass of the assembler.*/
static int handle_opcode_with_two_operand(LINE_INDICATION type, main_ptr *main_head, main_ptr *main_tail, const char* p, int start_address, const char *file_name, int line_number, int* error_flag) {
    char *label ;
    int count, character, soure_op, target_op, length ;
    count = DEFAULT_VALUE ;
    character = DEFAULT_VALUE ;
    soure_op = DEFAULT_VALUE ;
    target_op = DEFAULT_VALUE ;
    length  = DEFAULT_VALUE;

    if (is_it_empty_line(p)) {/* Case 1: Standing on the very first char after opcode word (White one for sure, otherwise didnt get this function).  */
        first_pass_general_error(MISSING_SOURCE_PARAMTER, file_name, line_number) ; 
        first_pass_general_error(MISSING_TARGET_PARAMTER, file_name, line_number) ; 
        *error_flag = TRUE ;
        return FALSE ;
    } 
    else if(check_next_non_white_char(p) == COMMA) { /* Case 2: Comma appear before parameter. */
        first_pass_general_error(ILEGAL_COMMA, file_name, line_number) ;
        *error_flag = TRUE ;
        return FALSE; 
    }
    else { /* Case 3: Not empty. */
        count = consecutive_white_characters(p) ;
        p += count ; /* Standing on the very first non white char after 'opcode_name' . */
        count = count_consecutive_non_white_or_until_first_comma(p) ; /* Length of operand. */
        character = check_next_non_white_char(p) ;
        /* Case 3.1: source is probably number. */
        if (character == MATH_SIGN || character == NUM) { /* Case 3.1: Source operand may be num. */
            if (type == LEA) { /* Case 3.1.1: 'lea' cant get num as source operand. */
                first_pass_general_error(UNAPROPIATE_SOURCE_PARAMETER, file_name, line_number) ; 
                *error_flag = TRUE ;
                return FALSE ;
            }
            soure_op = is_it_legal_number(p, count, file_name, line_number, error_flag, REGULAR_OPERAND) ;
            if (soure_op != ILEGAL) { /* Case 3.1.2: Num is legal. */
                    p += count ; /* p is on the first char after first operand (whites before comma / comma itself). */
                    count = count_until_next_operand(p, error_flag, file_name, line_number, ELSE) ; /* Else mean every type of operand type. */ 
                    if (count != ILEGAL) { /* Case 3.1.2.1: There is exactly one comma and there is target parameter. */
                        p += count ; /* p is on the first char of target operand. */
                        character = check_next_non_white_char(p) ; /* Checking which char. */
                        count = consecutive_non_white_characters(p) ; /* Length of target operand .*/
                        if (character == MATH_SIGN || character == NUM) { /* Case 3.1.2.1.1: Target operand may be another number.*/
                            if (type != CMP) { /* Case 3.1.2.1.1.1: Only 'cmp' can get num as target operand in this family. */
                                first_pass_general_error(UNAPROPIATE_TARGET_PARAMETER, file_name, line_number) ; 
                                *error_flag = TRUE ;
                                return FALSE ;
                            }
                            target_op = is_it_legal_number(p, count, file_name, line_number, error_flag, REGULAR_OPERAND) ;
                            if (target_op != ILEGAL) { /* Case 3.1.2.1.1.2: Num is legal, We have two num operands. */
                                p += count ; /* p is on the first char after target operand. */
                                if (is_it_empty_line(p)) { /* Case 3.1.2.1.1.2.1: ALL LEGAL.*/
                                    create_main_node(main_head, main_tail, type, IMMEDIATELY_ADDRESS, IMMEDIATELY_ADDRESS, start_address, NULL, NULL, file_name, line_number, DEFAULT_VALUE , DEFAULT_VALUE) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, IMMEDIATELY_ADDRESS, IMMEDIATELY_ADDRESS)) ;
                                    main_node_set_second_line_code(*main_tail, (short int)soure_op << 2) ; /* Num is 10 bits. */
                                    main_node_set_third_line_code(*main_tail, (short int)target_op << 2) ; /* Num is 10 bits. */
                                    return 3 ; /* Three memory cell. */
                                }
                                else { /* Case 3.1.2.1.1.2.2: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    return FALSE ;
                                }
                            }
                            else { /* Case 3.1.2.1.1.3: Ilegal num. Errors printed, flag updated.*/
                                return FALSE ;
                            }
                        }
                        else if (character == ATSIGN) { /* Case 3.1.2.1.2: Target operand may be register. */
                            p++ ; /* Sould standing on 'r' (after '@'). */
                            if (isspace(p[0])) { /* Case 3.1.2.1.2.1: Missing name of register after '@'. */
                                first_pass_general_error(NAME_OF_REGISTAR_IS_MISSING_AFTER_ATSIGN, file_name, line_number) ; 
                                *error_flag = TRUE ;
                                return FALSE ;
                            }
                            target_op = is_it_register_name(p, count-1,error_flag) ;  /* count -1 because we already increment p to 'r', count was included '@' before. */ 
                            p += count-1 ;
                            if (target_op != ILEGAL) { /* Case 3.1.2.1.2.2: Legal target register. */
                                if (is_it_empty_line(p)) { /* Case 3.1.2.1.2.2.1: ALL LEGAL. */
                                    create_main_node(main_head, main_tail, type, IMMEDIATELY_ADDRESS, DIRECT_REGISTER_ADDRESS, start_address, NULL, NULL, file_name, line_number, DEFAULT_VALUE , DEFAULT_VALUE) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, IMMEDIATELY_ADDRESS, DIRECT_REGISTER_ADDRESS)) ;
                                    main_node_set_second_line_code(*main_tail, (short int)soure_op << 2) ; /* Num is 10 bits. */
                                    main_node_set_third_line_code(*main_tail, (short int)target_op << 2) ; /* Target register */
                                    return 3 ; /* Three memory cell. */
                                }
                                else { /* Case 3.1.2.1.2.2.2: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    return FALSE ;
                                }
                            }
                            else { /* Case 3.1.2.1.2.3: Ilegal register name, not printed inside the calling function because of logical considerations, but update the error flag. */
                                first_pass_general_error(ILEGAL_REGISTER_NAME, file_name, line_number) ; 
                                return FALSE ;
                            }
                        }
                        else if (character == LETTER) { /* Case 3.1.2.1.3: Target operand may be label. */
                            if (!is_it_legal_label(p, count, file_name, line_number, error_flag)) {  /* Case 3.1.2.1.3.1: Ilegal label. Errors printed, flag updated.*/
                                return FALSE ;
                            }
                            else { /* Case 3.1.2.1.3.2: Legal label. */
                                if (!is_it_empty_line(p + count)) {  /* Case 3.1.2.1.3.2.1: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    return FALSE ;
                                }
                                else { /* Case 3.1.2.1.3.2.2: ALL LEGAL.*/
                                    create_main_node(main_head, main_tail, type, IMMEDIATELY_ADDRESS, DIRECT_ADDRESS, start_address, NULL, p, file_name, line_number, DEFAULT_VALUE , count) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, IMMEDIATELY_ADDRESS, DIRECT_ADDRESS)) ;
                                    main_node_set_second_line_code(*main_tail, (short int)soure_op << 2) ; /* Num is 10 bits. */
                                    return 3 ; /* Three memory cell, Third one saving for label. */
                                }
                            }
                        }
                        else { /* Case 3.1.2.1.4: Unapropiate target parameter. */
                            first_pass_general_error(UNAPROPIATE_TARGET_PARAMETER, file_name, line_number) ; 
                            *error_flag = TRUE ;
                            return FALSE ;
                        }
                    }
                    else { /* Case 3.1.2.2: count_until_next_operand indicate error, printed it, and update the flag. */
                        return FALSE ;
                    }
            }
            else { /* Case 3.1.3: Ilegal num. Errors printed, flag updated.*/
                return FALSE ;
            }
        }
        /* Case 3.2: source is probably register. */
        else if (character == ATSIGN) { /* Case 3.2: Source operand may be register. */
            if (type == LEA) { /*  Case 3.2.1: 'lea' cant get register as source operand. */
                first_pass_general_error(UNAPROPIATE_SOURCE_PARAMETER, file_name, line_number) ; 
                *error_flag = TRUE ;
                return FALSE ;
            }
            p++ ;
            if (isspace(p[0])) { /* Case 3.2.2: Missing name of register after '@'. */
                first_pass_general_error(NAME_OF_REGISTAR_IS_MISSING_AFTER_ATSIGN, file_name, line_number) ; 
                *error_flag = TRUE ;
                return FALSE ;
            }
            soure_op = is_it_register_name(p, count-1, error_flag) ; /* Count -1 because we dont count '@' char. */
            if (soure_op != ILEGAL) { /* Case 3.2.3: Register is legal. */
                    p += count-1 ; /* p is on the first char after first operand (whites before comma / comma itself). */
                    count = count_until_next_operand(p, error_flag, file_name, line_number, ELSE) ; /* Else mean every type of operand type. */ 
                    if (count != ILEGAL) { /* Case 3.2.3.1: There is exactly one comma and there is target parameter. */
                        p += count ; /* p is on the first char of target operand. */
                        character = check_next_non_white_char(p) ; /* Checking which char. */
                        count = consecutive_non_white_characters(p) ; /* Length of target operand .*/
                        if (character == MATH_SIGN || character == NUM) { /* Case 3.2.3.1.1: Target operand may be number.*/
                            if (type != CMP) { /* Case 3.2.3.1.1.1: Only 'cmp' can get num as target operand in this family. */
                                first_pass_general_error(UNAPROPIATE_TARGET_PARAMETER, file_name, line_number) ; 
                                *error_flag = TRUE ;
                                return FALSE ;
                            }
                            target_op = is_it_legal_number(p, count, file_name, line_number, error_flag, REGULAR_OPERAND) ;
                            if (target_op != ILEGAL) { /* Case 3.2.3.1.1.2: Num is legal.*/
                                p += count ; /* p is on the first char after target operand. */
                                if (is_it_empty_line(p)) { /* Case 3.2.3.1.1.2.1: ALL LEGAL.*/
                                    create_main_node(main_head, main_tail, type, DIRECT_REGISTER_ADDRESS, IMMEDIATELY_ADDRESS, start_address, NULL, NULL, file_name, line_number, DEFAULT_VALUE , DEFAULT_VALUE) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, DIRECT_REGISTER_ADDRESS, IMMEDIATELY_ADDRESS)) ;
                                    main_node_set_second_line_code(*main_tail, (short int)soure_op << 7) ; /* Num is 10 bits. */
                                    main_node_set_third_line_code(*main_tail, (short int)target_op << 2) ; /* Num is 10 bits. */
                                    return 3 ; /* Three memory cell. */
                                }
                                else { /* Case 3.2.3.1.1.2.2: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    return FALSE ;
                                }
                            }
                            else { /* Case 3.2.3.1.1.3: Ilegal num. Errors printed, flag updated.*/
                                return FALSE ;
                            }
                        }
                        else if (character == ATSIGN) { /* Case 3.2.3.1.2: Target operand may be register. */
                            p++ ;
                            if (isspace(p[0])) { /* Case 3.2.3.1.2.1: Missing register name after '@'. */
                                first_pass_general_error(NAME_OF_REGISTAR_IS_MISSING_AFTER_ATSIGN, file_name, line_number) ; 
                                *error_flag = TRUE ;
                                return FALSE ;
                            }
                            target_op = is_it_register_name(p, count-1,error_flag) ; 
                            p += count-1 ;
                            if (target_op != ILEGAL) { /* Case 3.2.3.1.2.2: Register target opernad is legal. */
                                if (is_it_empty_line(p)) { /* Case 3.2.3.1.2.2.1: ALL LEGAL.*/
                                    create_main_node(main_head, main_tail, type, DIRECT_REGISTER_ADDRESS, DIRECT_REGISTER_ADDRESS, start_address, NULL, NULL, file_name, line_number, DEFAULT_VALUE , DEFAULT_VALUE) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, DIRECT_REGISTER_ADDRESS, DIRECT_REGISTER_ADDRESS)) ;
                                    main_node_set_second_line_code(*main_tail, register_operands(soure_op, target_op)) ; /* Source register is on 7-11 bits. Target register is on 2-6 bits. */      
                                    return 2 ; /* Two memory cell. */
                                }
                                else { /* Case 3.2.3.1.2.2.2: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    return FALSE ;
                                }
                            }
                            else { /* Case 3.2.3.1.2.3: Ilegal register name, not printed inside the calling function because of logical considerations, but update the error flag. */
                                first_pass_general_error(ILEGAL_REGISTER_NAME, file_name, line_number) ; 
                                return FALSE ;
                            }
                        }
                        else if (character == LETTER) { /* Case 3.2.3.1.3: Target operand may be label. */
                            if (!is_it_legal_label(p, count, file_name, line_number, error_flag)) {  /* Case 3.2.3.1.3.1: Ilegal label. Errors printed, flag updated.*/
                                return FALSE ;
                            }
                            else { /* Case 3.2.3.1.3.2: Legal label. */
                                if (!is_it_empty_line(p + count)) { /* Case 3.2.3.1.3.2.1: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    return FALSE ;
                                }
                                else { /*  Case 3.2.3.1.3.2.2: ALL LEGAL.*/
                                    create_main_node(main_head, main_tail, type, DIRECT_REGISTER_ADDRESS, DIRECT_ADDRESS, start_address, NULL, p, file_name, line_number, DEFAULT_VALUE , count) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, DIRECT_REGISTER_ADDRESS, DIRECT_ADDRESS)) ;
                                    main_node_set_second_line_code(*main_tail, (short int)soure_op << 7) ; /* Source register is on 7-11 bits. */
                                    return 3 ; /* Three memory cell, Third one saving for label. */
                                }
                            }
                        }
                        else { /* Case 3.2.3.1.4: Unaprpotate target parameter. */
                            first_pass_general_error(UNAPROPIATE_TARGET_PARAMETER, file_name, line_number) ; 
                            *error_flag = TRUE ;
                            return FALSE ;
                        }
                    }
                    else { /* Case 3.2.3.2: count_until_next_operand indicate error, printed it, and update the flag. */
                        return FALSE ;
                    }
            }
            else { /* Case 3.2.4: Ilegal register name, not printed inside the calling function because of logical considerations, but update the error flag. */
                first_pass_general_error(ILEGAL_REGISTER_NAME, file_name, line_number) ; 
                return FALSE ;
            } 
        }
        /* Case 3.3: source is probably label. */ 
        else if (character == LETTER) { /* Case 3.3: Source operand may be Label. */
            label = malloc_for_char(count+1, file_name, line_number) ; /* Allocation will be checked at the function. */
            strncpy(label,p,count) ; /* Saving first parameter (label). */ 
            label[count] = '\0' ; /* And now we pad it. */
            length = count ; /* Saving first parameter (label) length. */
            if (is_it_legal_label(p, count, file_name, line_number, error_flag)) { /* Case 3.3.1: Register is legal. */                    
                    p += count ; /* p is on the first char after first operand (whites before comma / comma itself). */
                    count = count_until_next_operand(p, error_flag, file_name, line_number, ELSE) ; /* Else mean every type of operand type. */ 
                    if (count != ILEGAL) { /* Case 3.3.1.1: There is exactly one comma and there is target parameter. */
                        p += count ; /* p is on the first char of target operand. */
                        character = check_next_non_white_char(p) ; /* Checking which char. */
                        count = consecutive_non_white_characters(p) ; /* Length of target operand .*/
                        if (character == MATH_SIGN || character == NUM) { /* Case 3.3.1.1.1: Target operand may be number.*/
                            if (type != CMP) { /* Case 3.3.1.1.1.1: Only 'cmp' can get num as target operand in this family. */
                                first_pass_general_error(UNAPROPIATE_TARGET_PARAMETER, file_name, line_number) ; 
                                *error_flag = TRUE ;
                                if (label != NULL) { 
                                    free(label) ;
                                    label = NULL ;
                                }       
                                return FALSE ;
                            }
                            target_op = is_it_legal_number(p, count, file_name, line_number, error_flag, REGULAR_OPERAND) ;
                            if (target_op != ILEGAL) { /* Case 3.3.1.1.1.2: Num is legal. */
                                p += count ; /* p is on the first char after target operand. */
                                if (is_it_empty_line(p)) { /* Case 3.3.1.1.1.2.1: ALL LEGAL.*/
                                    create_main_node(main_head, main_tail, type, DIRECT_ADDRESS, IMMEDIATELY_ADDRESS, start_address, label, NULL, file_name, line_number, length , DEFAULT_VALUE) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, DIRECT_ADDRESS, IMMEDIATELY_ADDRESS)) ;
                                    /* Second line code is saved for label. */
                                    main_node_set_third_line_code(*main_tail, (short int)target_op << 2) ; /* Num is 10 bits. */
                                    if (label != NULL) {
                                        free(label) ;
                                        label = NULL ;
                                    }
                                    return 3 ; /* Three memory cell. */
                                }
                                else { /* Case 3.3.1.1.1.2.2: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    if (label != NULL) {
                                        free(label) ;
                                        label = NULL ;
                                    }
                                    return FALSE ;
                                }
                            }
                            else { /* Case 3.3.1.1.1.3: Ilegal num. Errors printed, flag updated.*/
                                if (label != NULL) {
                                    free(label) ;
                                    label = NULL ;
                                }
                                return FALSE ;
                            }
                        }
                        else if (character == ATSIGN) { /* Case 3.3.1.1.2: Target operand may be register. */
                            p++ ;
                            if (isspace(p[0])) { /* Case 3.3.1.1.2.1: Name of register is missing after '@'. */
                                first_pass_general_error(NAME_OF_REGISTAR_IS_MISSING_AFTER_ATSIGN, file_name, line_number) ; 
                                *error_flag = TRUE ;
                               if (label != NULL) {
                                    free(label) ;
                                    label = NULL ;
                                }
                                return FALSE ;
                            }
                            target_op = is_it_register_name(p, count-1,error_flag) ; 
                            p += count-1 ;
                            if (target_op != ILEGAL) { /* Case 3.3.1.1.2.2: Legal register name. */
                                if (is_it_empty_line(p)) { /*  Case 3.3.1.1.2.2.1: ALL LEGAL.*/
                                    create_main_node(main_head, main_tail, type, DIRECT_ADDRESS, DIRECT_REGISTER_ADDRESS, start_address, label, NULL, file_name, line_number, length , DEFAULT_VALUE) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, DIRECT_ADDRESS, DIRECT_REGISTER_ADDRESS)) ;
                                    /* Second line code is saved for label. */
                                    main_node_set_third_line_code(*main_tail, (short int)target_op << 2) ; /* Target register is on 2-6 bits. */
                                    if (label != NULL) {
                                        free(label) ;
                                        label = NULL ;
                                    }
                                    return 3 ; /* Three memory cell. */
                                }
                                else { /* Case 3.3.1.1.2.2.2: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    if (label != NULL) {
                                        free(label) ;
                                        label = NULL ;
                                    }
                                    return FALSE ;
                                }
                            }
                            else { /* Case 3.3.1.1.2.3: Ilegal register name, not printed inside the calling function because of logical considerations, but update the error flag. */
                                first_pass_general_error(ILEGAL_REGISTER_NAME, file_name, line_number) ; 
                                if (label != NULL) {
                                    free(label) ;
                                    label = NULL ;
                                }
                                return FALSE ;
                            }
                        }
                        else if (character == LETTER) { /* Case 3.3.1.1.3: Target operand may be label. */
                            if (!is_it_legal_label(p, count, file_name, line_number, error_flag)) {  /* Case 3.3.1.1.3.1: Ilegal label. Errors printed, flag updated.*/
                                if (label != NULL) {
                                    free(label) ;
                                    label = NULL ;
                                }
                                return FALSE ;
                            }
                            else { /* Case 3.3.1.1.3.2: Legal label. */
                                if (!is_it_empty_line(p + count)) { /* Case 3.3.1.1.3.2.1: Unrelated text at the end of line. */
                                    first_pass_general_error(ADDITIONAL_UNRELATED_TEXT, file_name, line_number) ; 
                                    *error_flag = TRUE ;
                                    if (label != NULL) {
                                        free(label) ;
                                        label = NULL ;
                                    }
                                    return FALSE ;
                                }
                                else { /* Case 3.3.1.1.3.2.2: ALL LEGAL.*/
                                    create_main_node(main_head, main_tail, type, DIRECT_ADDRESS, DIRECT_ADDRESS, start_address, label, p, file_name, line_number, length , count) ;
                                    main_node_set_first_line_code(*main_tail, operands_line_code(type, DIRECT_ADDRESS, DIRECT_ADDRESS)) ;
                                    if (label != NULL) {
                                        free(label) ;
                                        label = NULL ;
                                    }
                                    return 3 ; /* Three memory cell. Second and third line code is saved for label. */ 
                                }
                            }
                        }
                        else { /* Case 3.3.1.1.4: Unapropiate target parameter.*/
                            first_pass_general_error(UNAPROPIATE_TARGET_PARAMETER, file_name, line_number) ; 
                            *error_flag = TRUE ;
                            if (label != NULL) {
                                free(label) ;
                                label = NULL ;
                            }
                            return FALSE ;
                        }
                    }
                    else { /* Case 3.3.1.2: count_until_next_operand indicate error, printed it, and update the flag. */
                        if (label != NULL) {
                            free(label) ;
                            label = NULL ;
                        }
                        return FALSE ;
                    }
            }
            else { /* Case 3.3.2: Ilegal label name, already printed inside the calling function because and flag was update. */
                if (label != NULL) {
                    free(label) ;
                    label = NULL ;
                }
                return FALSE ;
            }
        }
        /* Case 3.4: source is ilegal. */
        else { /* Ilegal source. */
            first_pass_general_error(UNAPROPIATE_SOURCE_PARAMETER, file_name, line_number) ; 
            *error_flag = TRUE ;
            return FALSE ;
        }
    }
}

static void handle_first_pass_line(int *address, int *ic, int *dc, char *line_p, main_ptr *main_head, main_ptr *main_tail, data_image_ptr *data_head, data_image_ptr *data_tail, def_label_ptr *def_head, int *error_flag, const char *file_name, int line_number) {
    LINE_INDICATION line_case ; 
    int length ;
    int temp_address ; 
    int label_case_count ;
    int label_flag ;
    int count ;
    label_flag = FALSE ;
    label_case_count = DEFAULT_VALUE ;
    count = DEFAULT_VALUE ;
    length = DEFAULT_VALUE ;
    temp_address = *address ;
    
    if (line_p[0] == ' ') { /* Case 1: Its has to be ilegal comment line because All lines have been shortened to be starting from the first character at the time of macro spreading,
    except for invalid comment lines (';' not first as required). Also all white line deleted. */
        *error_flag =  TRUE ;
        first_pass_general_error(COMMENT_LINE_ILLEGAL_SPACES, file_name, line_number) ;
        return ; 
    }
    length = consecutive_non_white_characters(line_p) ;
    line_case = line_indication(line_p, length, error_flag, file_name, line_number) ;
    if (line_case != ILEGAL) { /* Case 2: Legal first sequence. starting parse.*/
        if (line_case == LABEL) { /* Case 2.1: Label case. */
            label_flag = TRUE ;
            label_case_count = handle_def_label_line(line_p, length, error_flag, file_name, line_number, def_head, temp_address) ;
            if (label_case_count != ILEGAL) { /* Case 2.1.1: Keep handling the line there is entry/extern/string/data/opcode statment from p+count so we increment p by 'count' chars. */
                line_p += label_case_count ;
                count = consecutive_non_white_characters(line_p) ;
                line_case =  line_indication(line_p, count, error_flag, file_name, line_number) ;
                if (line_case != ILEGAL) { /* Case 2.1.1.1: Legal, keep handlong */
                    line_p += count ; /* Standing on very first char after opcode/string/data! its not extern or entry (already check in first calling to line_indication) */
                }   
                else { /* Case 2.1.1.2: Ilegal! All updated. error printed.   */    
                    return ;
                }    
            }    
            else { /* Case 2.1.2: Ilegal or Label defined nothing (warning), error/warrning printed, flag was updated (for error case), getting next line, anyway- Second pass will not going to happen.*/
                return ;
            }
        }
        if (label_flag == FALSE) { 
            line_p += length ; /* Standing on first char after first non white sequence chars. */
        } /* Else already increment by 'label_case_count' . */
        /* Keep handling. */    
        /*  #OPEN For next 3 cases: */
        /* If count == FALSE == 0, its "else case", error printed, flag was updated, and increasing by zero means nothing. */
        if (line_case == RTS || line_case == STOP) { /* Case 2.2: rts/stop opcodes. */
            count = handle_opcode_without_operands(line_case, main_head, main_tail, line_p, temp_address, file_name, line_number, error_flag) ; 
            *address += count ; 
            *ic +=  count ;
            return ; 
        }
        else if ((line_case >= MOV && line_case <= SUB) || line_case == LEA) { /* Case 2.2: lea/mov/cmp/sub (Two operands opcodes). */
            count = handle_opcode_with_two_operand(line_case, main_head, main_tail,line_p, temp_address, file_name, line_number, error_flag) ;
            *address += count ;
            *ic +=  count ;
            return ; 
        }
        else if (line_case >= NOT && line_case <= JSR) { /* Case 2.3: not/clr/inc/dec/jmp/bne/red/prn/jsr (Only one operand at target opcodes.)
        (Dont worry, LEA is in the middle but already checked. So its worked well). */
            count = handle_opcode_with_one_operand(line_case, main_head, main_tail,line_p, temp_address, file_name, line_number, error_flag) ;
            *address += count ;
            *ic +=  count ;
            return ; 
        }
        /* #CLOSE */
        else if (line_case == ENTRY) { /* Case 2.4: '.entry'. */
            handle_entry_line(line_p, error_flag, file_name, line_number, def_head, temp_address) ;
            return ; 
        }
        else if (line_case == EXTERN) { /* Case 2.5: '.extern'. */
            handle_extern_line(line_p, error_flag, file_name, line_number, def_head, temp_address) ;
            return ; 
        }
        else if (line_case == STRING ) {/* Case 2.6: '.string' */
            count = handle_string_line(line_p, error_flag, file_name, line_number, main_head, main_tail, data_head, data_tail, temp_address) ;
            *address += count ; 
            *dc +=  count ;
            return ; 
        }
        else  { /* Case 2.7: '.data'. */ 
            count = handle_data_line(line_p, error_flag, file_name, line_number, main_head, main_tail, data_head, data_tail, temp_address) ;
            *address += count ; 
            *dc +=  count ;
        }
    }
    else { /* Case 3: Ilegal, error printed, flag was updated, getting next line, anyway- Second pass will not going to happen.*/
        return ;
    }
}

int handle_first_pass(const char * file_name, FILE *process_f, FILE *ob_f, FILE *ext_f, FILE *ent_f) {
    main_ptr main_head = NULL ;
    main_ptr main_tail = NULL ;
    data_image_ptr data_head = NULL ;
    data_image_ptr data_tail = NULL ;
    def_label_ptr def_head = NULL ; /* No need for a tail for logical reasons. */
    char line[MAX_LINE] ;
    int line_number ;  
    int errors_flag ;
    int address ;
    int ic ;
    int dc ;
    address = FIRST_MEMORY_CELL ; /* Mamman instruction: memory start from 100 max is 1024, first 100 cells (0-99) are from system. */
    ic = DEFAULT_VALUE ;
    dc = DEFAULT_VALUE ;
    line_number = 1 ;
    errors_flag = FALSE ;

    while ((fgets(line, MAX_LINE, process_f) != NULL)) {        
            handle_first_pass_line(&address, &ic, &dc, &line[DEFAULT_VALUE], &main_head, &main_tail, &data_head, &data_tail, &def_head, &errors_flag, file_name, line_number) ;
            if (address > MAX_MEMORY_CELL) { /* Fatal error. No more memory. */
                memory_error(MAXIMUM_MEMORY_EXCEEDED, file_name, line_number) ;
                free_all_first_pass_lists(&main_head, &data_head, &def_head) ; /* Will be check at function if not NULL. */
                return FALSE ;
            }
            ++line_number ; 
            continue ;   
    }
    /* Loop is finished, check if error indicate in first pass, if indicate - return FALSE and not make second_pass. If not indicate- start second pass proccess. */
    if (errors_flag == TRUE)  {
        free_all_first_pass_lists(&main_head, &data_head, &def_head) ;
        printf("\n\nErrors encountered while parsing the 'am' file (In File: %s).\nObject('.ob'), external('.ext'), entry('.ent') files will not be created.\nGetting next input file (if exist).\n", file_name) ;
        return FALSE ;
    }
    if(!ferror(process_f)) { /* There is no error with the file. */
        if (errors_flag == FALSE) {
            /* Calling second pass. */
            if (handle_second_pass(ob_f, ext_f, ent_f, &main_head, &data_head, &def_head, ic, dc, file_name)) { /* Second pass worked well all file created. Done procces for this file, getting next input file. */
                free_all_first_pass_lists(&main_head, &data_head, &def_head) ;
                return TRUE ;
            }
            else { /* Second_pass return FAlSE, error indicate and printed, proccess for this file is done. getting next file if exist. */
                printf("\n\nErrors encountered while parsing the 'am' file (In File: %s).\nObject('.ob'), external('.ext'), entry('.ent') files will not be created.\nGetting next input file (if exist).\n", file_name) ;
                free_all_first_pass_lists(&main_head, &data_head, &def_head) ;/* Will be check at function if not NULL. */
                return FALSE ;
            }
        }
        free_all_first_pass_lists(&main_head, &data_head, &def_head) ;
        return FALSE ;  /* else: errors_flag == TRUE  ---> Second pass will not happend.*/             
    } 
    /* else: error indicator is set. Should never reach here. */
    file_error(ERROR_INDICATOR_IS_SET, file_name, line_number) ;
    free_all_first_pass_lists(&main_head, &data_head, &def_head) ;/* Will be check at function if not NULL. */
    return FALSE ;
}
