/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * -------------------------
 * File name: handle_macro.c
 * -------------------------
 * This file is responsible for:
 * - Preprocessing macros by expanding them and checking for errors prior to the first pass.
 * - Managing storage and retrieval of macro-related data.
 * - Ensuring that macros are correctly defined and spread across the source code.
 * -------------------------
 * Instructions from the forum:
 * - A macro cannot be called before its definition line, error will be printed.
 * - A macro cannot redefined, error will be printed.
 * - Empty lines will be deleted and will not appear in the '.am' file (if created).
 * - White characters at the beginning of a line will be deleted and will not appear in the '.am' file.
 * - Legal comment line (first char is ';' and no white spaces before) will be deleted and will not appear in the '.am' file. (Ilegal will appear and error will be printed at first pass process).
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/error.h"
#include "headerfiles/first_pass_helpers.h"
#include "headerfiles/general_helpers.h"
#include "headerfiles/output_files.h"
#include <stdio.h>
#include <string.h>

/* Functions: */
static int is_it_macro_name(const char *start, int max, macro_ptr *head) {
    macro_ptr temp = NULL;
    int len;
    len = DEFAULT_VALUE;
    temp = *head;
    while (temp != NULL) {
        len = strlen(macro_node_get_macro_name(temp));
        if ((max == len) && (strncmp(macro_node_get_macro_name(temp), start, max) == DEFAULT_VALUE)) { /* (max == len check is to avoid substring equalization.*/
            return TRUE;
        }
        temp = macro_node_get_next(temp);
    }
    return FALSE;
}

static int is_it_macro_def(const char *start, int max) { 
    char macro[MCRO_DEF_LENGTH] = "mcro"; /* Auto pad with '\0'. */
    if ((max == strlen(macro)) && (strncmp(macro, start, max) == DEFAULT_VALUE)) { /* (max == strlen(macro) check is to avoid substring equalization.*/
        return TRUE ;
    }
    return FALSE ;
}

static int is_it_end_macro (const char *start, int max) {
    char endmacro[ENDMCRO_LENGTH] = "endmcro" ; /* Auto pad with '\0'. */
    if ((max == strlen(endmacro)) && (strncmp(endmacro, start, max) == DEFAULT_VALUE)) { /* (max == strlen(endmacro) check is to avoid substring equalization.*/
        return TRUE ;
    }
    return FALSE ; 
}

static int one_sequence_case(char *line_p, const char *file_name, int line_number, macro_ptr * head, macro_ptr *tail, int *macro_def_flag, int *errors_flag, int count) {    
    if (is_it_function_name(line_p, count) >= DEFAULT_VALUE ) { /* Case 4.1: (-999 indicate false) If the function is missing parameters the error will be detected in first pass (Mamman instructions), -1 will be "FALSE" this time. */
        return COPY_AS_IS ; 
    }
    else if (is_it_directive_name(line_p, count) >= DEFAULT_VALUE ) { /* Case 4.2: (-999 indicate false) If the directive is missing parameters the error will be detected in first pass (Mamman instructions), -1 will be "FALSE" this time. */ 
        return COPY_AS_IS ; 
    }
    else if (is_it_end_macro(line_p, count)) { /* Case 4.3: */ 
        if (*macro_def_flag == TRUE) { /* Case 4.3.1: Legal closing with endmcro. (check if was a def).*/
            create_macro_node(head, tail, FALSE,TRUE, line_p, file_name, line_number) ;   
            *macro_def_flag = FALSE ;
            return DONT_COPY ;
        }
        else { /* Case 4.3.2: "endmcro" without start def of mcro lines before. */
            *errors_flag = TRUE ;
            macro_pass_error(END_MACRO_WITHOUT_DEF, file_name, line_number) ;
            return COPY_AS_IS ;
        }
    }
    else if (is_it_macro_name(line_p,count ,head)) {  /* Case 4.4: already defined (Will be spreded). */
        return MACRO_NAME ;
    }
    else if (is_it_macro_def(line_p, count)) { /* Case 4.5: Macro definition without a name??? */
        *errors_flag = TRUE ;
        macro_pass_error(MACRO_DEF_WITHOUT_NAME, file_name, line_number) ;
        return COPY_AS_IS ;
    }
    else { /* Case 4.6: Not Emptyline/label/comment_line/function_name/directive_name/end_mcro -Its has to be mcro name that not defined yet. */
        *errors_flag = TRUE ;
        macro_pass_error(NO_DEF_FOR_MACRO, file_name, line_number) ;
        return DONT_COPY ;
    }       
}     

static int more_than_one_sequence_case(char * line_p, const char * file_name, int line_number, macro_ptr * head, macro_ptr * tail, int * macro_def_flag, int *errors_flag, int count){    
    if (is_it_directive_name(line_p, count) >= DEFAULT_VALUE) { /* Case 5.1:  Is the name equal to function ? , -999 will be "FALSE" this time. */
        *errors_flag = TRUE ;
        macro_pass_error(NAME_OF_MACRO_LIKE_DIRECTIVE, file_name, line_number) ;
        return COPY_AS_IS ;
    }
    else if (is_it_function_name(line_p, count) >= DEFAULT_VALUE) { /* Case 5.2: Is the name equal to directive name ?, -999 will be "FALSE" this time. */
        *errors_flag = TRUE ;
        macro_pass_error(NAME_OF_MACRO_LIKE_FUNCTION, file_name, line_number) ;
        return COPY_AS_IS ;
    }
    else if (line_p[0] == '@' && is_it_register_name(&line_p[1],count-1,errors_flag) != ILEGAL) { /* index one because we checked if first index is '@' and count-1 for same reason. */
        macro_pass_error(NAME_OF_MACRO_LIKE_REGISTER, file_name, line_number) ; /* Error flag is updated at function because of logical reason related to first pass. */
        return COPY_AS_IS ;
    }
    else { /* Case 5.3: check if its empty after name (Legal) in defined macro line. */
        if (!is_it_empty_line(line_p+count)) { /* Case 5.3.1: After ' mcro   name ' line should be empty! - Illegal*/
            *errors_flag = TRUE ;
            macro_pass_error(DEF_LINE_ISNT_EMPTY_AFTER_NAME, file_name, line_number) ;         
            return COPY_AS_IS ;
        } 
        else { /* Case 5.3.2: Legal macro defined line or redefined macro. */
            if (is_it_macro_name(line_p, count,head)) { /* Case 5.3.2.1: Redefined macro ?*/
                macro_pass_error(MACRO_CANT_REDEFINED, file_name, line_number) ;
                *errors_flag = TRUE ;
                *macro_def_flag = FALSE ;
                return COPY_AS_IS ;
            }
            else { /* Case 5.3.2.2: Legal macro defined line. */
                *macro_def_flag = TRUE ;
                create_macro_node(head, tail, TRUE, FALSE, line_p, file_name, line_number) ;
                return DONT_COPY ;
            } 
        }
    }
}

static int handle_as_file_line(char * line_p, const char * file_name, int line_number, macro_ptr * head, macro_ptr * tail,  int * macro_def_flag, int * errors_flag ) {
    int count ;
    int w ;
    w = DEFAULT_VALUE ;
    count =  consecutive_non_white_characters(line_p) ; /* How many non white in a row. */   
    /* Cases:*/
    if (!(is_it_end_macro(line_p,count))  && (*macro_def_flag == TRUE)) { /* Case 2: Inside  macro content, include empty lines */ 
        create_macro_node(head, tail, FALSE, FALSE, line_p, file_name, line_number) ;
        return DONT_COPY ;
    }
    else if ((count > DEFAULT_VALUE) && (line_p[count-1] == ':')) { /* Case 3: If the label is missing parameter or more than 31 chars (Mamman instructions- limit) the error will be detected in first pass (Mamman instructions). */ 
        return COPY_AS_IS ;
    }
    /* As we know, macros name (or end_mcro) are the only one characters sequence in the line, so first im going to check if its macro name indicate. 
       Only one characters sequence and not function-name/comment/directive/label(end with ':'). */
    else if (is_it_empty_line(line_p+count)) {/* Case 4: Only one chars sequence/one char which is not white char in the line. */
        return one_sequence_case(line_p, file_name, line_number, head, tail, macro_def_flag, errors_flag,count) ;
    }
    /* There is more than one sequence(at least one char which is not white char) in the line */
    else if (is_it_macro_def(line_p, count)) { /* Case 5: There is name after "mcro" Lets check if legal: */
        line_p += count ; /* Increment pointer to first white char after "mcro". */
        w = consecutive_white_characters(line_p)  ; /* Counting white char untill next non white char. */
        line_p += w ; /* Increment pointer to first non white char after "mcro" (first char of possible name of mcro). */
        count = consecutive_non_white_characters(line_p) ; /* Counting length of possible name of macro.*/
        return more_than_one_sequence_case(line_p, file_name, line_number, head, tail, macro_def_flag, errors_flag, count) ;
    }
    else if (is_it_macro_name(line_p, count, head)) { /* Case 6: If its not empty line after first sequence (we know that thanks to case 4 checking ) so we have to check if maybe its a macro name but there is something after it -  Illegal! */
        macro_pass_error(MACRO_NAME_LINE_ISNT_EMPTY_AFTER_NAME, file_name, line_number) ;
        *errors_flag = TRUE ;
        return COPY_AS_IS ;
    }
    else if (is_it_end_macro(line_p, count)) { /* Case 7: If its not empty line after first sequence (we know that thanks to case 4 checking ) so we have to check if maybe its a endmcro but there is something after it -  Illegal */         
        macro_pass_error(END_MACRO_LINE_ISNT_EMPTY_AFTER_ENDMCRO, file_name, line_number) ;
        *errors_flag = TRUE ;
        *macro_def_flag = FALSE ; /* We are closing it so we could reach more errors if exist.*/
        return COPY_AS_IS ;
    } 
    else if (*macro_def_flag == FALSE /*&& macro_end_flag == FALSE*/) { /* Case 8: Regular line. */
        return COPY_AS_IS ;
    } 
    else { /* Case 9: Should never reach here. */
        return DONT_COPY ;
    }
}

int handle_as_file(const char * file_name, FILE * open_f , FILE * process_f) {
    macro_ptr head = NULL ;
    macro_ptr tail = NULL ;
    char line[MAX_LINE] ; 
    int handle_line_case ; 
    int line_number ;  
    int errors_flag ;
    int macro_def_flag ; 
    int white_chars ;
    line_number = 1 ;
    white_chars = DEFAULT_VALUE ;
    handle_line_case = DEFAULT_VALUE ;
    errors_flag = FALSE ;
    macro_def_flag = FALSE ;

    while ((fgets(line, MAX_LINE, open_f) != NULL)) {        
        if ((line[(strlen(line))-1] == '\n') || feof(open_f)){ /* Not bigger than 80. */            
            white_chars = consecutive_white_characters(line)  ;  
            if (line[white_chars] == '\0') { /* Empty line- delete it and Getting next line... */
                handle_line_case = DONT_COPY ;
            }
            else if (line[white_chars] == ';'){
                if (white_chars > DEFAULT_VALUE) { /* Illegal comment line, some white chars before ';' - will be revealed on the first pass . */
                    white_chars = DEFAULT_VALUE ; /* Because i want to spread it as-is and detect the error at first pass (Mamman instructions). */
                    handle_line_case = COPY_AS_IS ;
                } 
                else { /* white_chars == DEFAULT_VALUE  ----> Legal comment line ---> delete. */
                    handle_line_case = DONT_COPY ;
                }
            }
            else {
                handle_line_case = handle_as_file_line(&line[white_chars], file_name, line_number, &head, &tail, &macro_def_flag, &errors_flag ) ;
            }           
            spread_am_file_line(handle_line_case, process_f, &head, &line[white_chars], &macro_def_flag, file_name, line_number) ; /* Spreading line in '.am' file. */
            ++line_number ; /* Line counting starting from 1,  Used for printing exact line errors (if exist). */ 
            continue ;
        }    
        else { /* We should never reach here because the Mamman assumption is that there is not more than maximum of 81 characters per line (include '\0' char). */
            errors_flag = TRUE ;
            memory_error(LINE_TOO_LONG, file_name, line_number) ;
            continue ;
        }  
    } 
    if(!ferror(open_f)) { /* There is no error with the file. */
        if (head != NULL) {
            free_macro_list(&head) ;
        }    
        if (errors_flag == FALSE) {
            return TRUE ;
        }
        printf("\n\nErrors encountered while parsing the source file (In File: %s).\nNo files will be created.\nGetting next input file (if exist).\n", file_name) ;
        return FALSE ;  /* else: errors_flag == TRUE */             
    } 
    /* else: error indicator is set. Should never reach here. */
    file_error(ERROR_INDICATOR_IS_SET, file_name, line_number) ;

    if (head != NULL) {
        free_macro_list(&head) ;
    }  
    return FALSE ;
}
