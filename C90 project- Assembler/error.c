/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * Date of Submission: 10/08/2023
 * -------------------------
 * File name: error.c
 * -------------------------
 * This file is responsible for:
 * - Printing different types of errors encountered during the assembler process.
 * - Formatting and displaying error messages to the user.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/error.h"
#include <stdio.h>

/* Functions: */
void warning(Warning warning, const char * file_name, int line_number) {
    printf("\nWarning!  -----  In File: %s  -----  In line number: %d  -----  Error type:\n", file_name, line_number) ;
    switch (warning) {
        case LABEL_BEFORE_ENTRY:
            printf("No label should appear before '.entry' directive sentence. \n") ;
            break ;
        case LABEL_BEFORE_EXTERN:
            printf("No label should appear before '.extern' directive sentence. \n") ;
            break ;
        case STRING_IS_EMPTY:
            printf("'.string' directive sentence is empty. \n") ;
            break ;
        case DATA_IS_EMPTY:
            printf("'.data' directive sentence is empty. \n") ;
            break ;
        case LABEL_DEFINED_NOTHING:
            printf("The label defined nothing- No content at all, the label will not be saved. \n") ;    
            break;
        case EXTERN_STATEMENT_TWICE:
            printf("Declaration of extern label twice is unnecessary. \n") ;    
            break;
        case ENTRY_STATEMENT_TWICE:
            printf("Declaration of entry label twice is unnecessary. \n") ;    
            break;
        }
}

void file_error(FileError error, const char * file_name, int line_number) {
    printf("\nAn error occurred!  -----  In File: %s  -----  In line number: %d  -----  Error type:\n", file_name, line_number) ;
    switch (error) {
        case FILE_OPEN_ERROR:
            printf("Can't open file. Please check if the names of the files you entered as input match the files you have.\n") ;
            break ;
        case READING_ERROR:
            printf("Fatal error! Error indicator became set while reading file.\n") ;
            break ;
        case DELETE_FILE_ERROR:
            printf("Fatal error! Error while trying delete the file.\n") ;
            break ;
        case ERROR_INDICATOR_IS_SET:
            printf("Fatal error! Error indicator become set.\n") ;
            break ;
    }
}

void macro_pass_error(MacroPassError error, const char * file_name, int line_number) {
    printf("\nAn error occurred! -----  In File: %s  -----  In line number: %d  -----  Error type:\n", file_name, line_number) ;
    switch (error) {
        case END_MACRO_WITHOUT_DEF:
            printf("Can't close mcro without open one.\n") ;
            break ;
        case NO_DEF_FOR_MACRO:
            printf("Can't spread mcro without definition.\n") ;
            break ;
        case MACRO_DEF_WITHOUT_NAME:
            printf("Can't define mcro without giving a name.\n") ;
            break ;
        case NAME_OF_MACRO_LIKE_DIRECTIVE:
            printf("Name of macro cant be like directive.\n") ;
            break ;
        case NAME_OF_MACRO_LIKE_FUNCTION:
            printf("Name of macro cant be like function.\n") ;
            break ;
        case NAME_OF_MACRO_LIKE_REGISTER:
            printf("Name of macro cant be like register.\n") ;
            break;
        case MACRO_NAME_LINE_ISNT_EMPTY_AFTER_NAME:
            printf("The macro spreading line should be empty (Only white chars is legal) after ' $Macro-name$ '.\n") ;
            break ;
        case DEF_LINE_ISNT_EMPTY_AFTER_NAME:
            printf("The defintion macro command line should be empty (Only white chars is legal) after 'mcro $Macro-name$'.\n") ;
            break ;
        case END_MACRO_LINE_ISNT_EMPTY_AFTER_ENDMCRO:
            printf("The end macro command line should be empty (Only white chars is legal) after ' endmcro '.\n") ;
            break ;
        case MACRO_CANT_REDEFINED:
            printf("Macro can not redefined.\n") ;
            break ;
    }
}

void first_pass_general_error(FirstPassGeneralError error, const char* file_name, int line_number) {
    printf("\nAn error occurred! ----- In File: %s ----- In line number: %d ----- Error type:\n", file_name, line_number);
    switch (error) {
        case COMMENT_LINE_ILLEGAL_SPACES:
            printf("Comment line char ' ; ' should be the first char in the line - without white chars before at all.\n");
            break;
        case ADDITIONAL_UNRELATED_TEXT:
            printf("Unrelated characters at the end of the text.\n");
            break;
        case TWO_CONSECUTIVE_COMMAS:
            printf("Illegal two consecutive commas.\n");
            break;
        case ILEGAL_COMMA:
            printf("Illegal place for a comma.\n");
            break;
        case MISSING_COMMA:
            printf("Missing comma after a parameter.\n");
            break;
        case MISSING_SOURCE_PARAMTER:
            printf("Missing source parameter.\n");
            break;
        case MISSING_TARGET_PARAMTER:
            printf("Missing target parameter.\n");
            break;
        case A_NUMBER_SHOULD_APPEAR_AFTER_PLUS_OR_MINUS:
            printf("A number must appear after '+' or '-'.\n");
            break;
        case UNAPROPIATE_SOURCE_PARAMETER:
            printf("Unappropriate source parameter.\n");
            break;
        case UNAPROPIATE_TARGET_PARAMETER:
            printf("Unappropriate target parameter.\n");
            break;
        case ONLY_INTEGERS_NUMBERS_ARE_ACCEPT:
            printf("Only integer numbers are acceptable.\n");
            break;
        case NAME_OF_REGISTAR_IS_MISSING_AFTER_ATSIGN:
            printf("Missing register name after '@'.\n");
            break;
        case ILEGAL_REGISTER_NAME:
            printf("Register name should be only one of those: 'r1' 'r2' 'r3' 'r4' 'r5' 'r6' 'r7'.\n");
            break;
    }
}

void first_pass_label_error(FirstPassLabelError error, const char* file_name, int line_number) {
    printf("\nAn error occurred! ----- In File: %s ----- In line number: %d ----- Error type:\n", file_name, line_number);
    switch (error) {
        case LABEL_NAME_SHOULD_START_WITH_LETTER:
            printf("Label name should start with a letter.\n");
            break;
        case LABEL_NAME_CONTAIN_ONLY_LETTERS_AND_NUMBERS:
            printf("Label name should contain only letters and numbers.\n");
            break;
        case LABEL_MAX_LENGTH_IS_31:
            printf("Label max length is 31.\n");
            break;
        case LABEL_NAME_CANT_BE_AS_OPCODE_NAME:
            printf("Label name can't be an OPCODE name.\n");
            break;
        case LABEL_NAME_CANT_BE_AS_REGISTAR_NAME:
            printf("Label name can't be a register name.\n");
            break;
        case LABEL_NAME_CANT_BE_AS_DIRECTIVE_NAME:
            printf("Label name can't be a directive name.\n");
            break;
        case LABEL_NAME_CANT_REDEFINED:
            printf("Label name can't be redefined.\n");
            break;
        case LABEL_CANT_BE_BOTH_EXTERNAL_AND_ENTRY:
            printf("Label can't be both external and entry.\n");
            break;
        case LABEL_CANT_DEFINED_LABEL:
            printf("Label name can't define a label.\n");
            break;
        case TWO_CONSECUTIVE_COLON:
            printf("Illegal two consecutive colons.\n");
            break;
        case MISSING_COLON:
            printf("A label name must end with consecutive colons ':'.\n");
            break;
    }
}

void first_pass_directive_error(FirstPassDirectiveError error, const char* file_name, int line_number) {
    printf("\nAn error occurred! ----- In File: %s ----- In line number: %d ----- Error type:\n", file_name, line_number);
    switch (error) {
        case TWO_CONSECUTIVE_DOT:
            printf("Illegal two consecutive dots.\n");
            break;
        case EXTERNAL_LABEL_CANT_BE_REDEFINED_IN_A_FILE:
            printf("External label can't be redefined in a file.\n");
            break;
        case MISSING_LABEL_AFTER_COMMA_OR_ILEGAL_COMMA_AT_END:
            printf("Missing label after a comma or delete comma at the end.\n");
            break;
        case EXTERN_IS_EMPTY:
            printf("At least one label name should appear after '.extern'.\n");
            break;
        case ENTRY_CONTAIN_ONE_LABEL_FOLLOWED_BY_NOTHING:
            printf("Entry ('.entry') statement should contain only one label followed by nothing.\n");
            break;
        case ENTRY_IS_EMPTY:
            printf("Label name should appear after '.entry'.\n");
            break;
        case DATA_LINE_OPERANDS_ARE_INTEGER:
            printf("Data line operands should be integer numbers and commas between them.\n");
            break;
        case MISSING_NUM_AFTER_COMMA_OR_ILEGAL_COMMA_AT_END:
            printf("A number must appear after a comma in the data line or delete comma at the end.\n");
            break;
        case STRING_SHOULD_START_WITH_PAIRS_OF_QUOTATION_MARKS:
            printf("String should start with pairs of quotation marks.\n");
            break;
        case STRING_SHOULD_END_WITH_PAIRS_OF_QUOTATION_MARKS:
            printf("String should end with pairs of quotation marks.\n");
            break;
        case STRING_DIRECTIVE_CONTAIN_ONE_STRING_FOLLOWED_BY_NOTHING:
            printf("String directive line contains one string followed by nothing.\n");
            break;
    }
}

void second_pass_error(SecondPassError error, const char * file_name, int line_number) {
    printf("\nAn error occurred!  -----  In File: %s  -----  In line number: %d  -----  Error type:\n", file_name, line_number) ; 
    switch (error) {
        case NO_SUCH_LABEL_DEFINED:
            printf("No label defined.\n");
            break ;
        case ENTRY_LABEL_WASNT_DEFINED_AT_FILE:
            printf("Entry label wasn't defined at file.\n") ;
            break ;
    }
}

void memory_error(MemoryError error, const char * file_name, int line_number) {
    printf("\nAn error occurred!  -----  In File: %s  -----  In line number: %d  -----  Error type:\n", file_name, line_number) ;
    switch (error) {
        case MACRO_ALLOCATION_FAILED:
            printf("Macro element memory allocation failed.'.am' file will not be created and also:\n") ;
            break ;
        case CHAR_ARRAY_ALLOCATION_FAILED:
            printf("char array memory allocation failed.\n");
            break ;
        case LINE_TOO_LONG: /* This one will detect in macro pass.*/
            printf("Line is grater then 80.'.am' file will not be created and also: \n") ;
            break ;
        case MAXIMUM_MEMORY_EXCEEDED:
            printf("An exception to the existing memory size (1024 cells) was detected.\n"); 
            break ;
        case MAX_REGISTER:
            printf("There are 8 registers and their names are: - r0,r1,...r7.\n");
            break ;
        case MAX_OPERAND_INTEGER:
            printf("INTEGERS in non data line RANGE is -512 to 511 (10 Bits).\n");
            break ;
        case MAX_DATA_INTEGER:
            printf("INTEGERS in data line RANGE is -2048 to 2047 (12 Bits).\n");
            break ;
        case DEF_LABEL_ALLOCATION_FAILED:
            printf("Defined Label table element memory allocation failed.\n") ;
            break ;
        case USED_LABEL_ALLOCATION_FAILED:
            printf("Used Label table element memory allocation failed.\n") ;
            break ;
        case MAIN_ALLOCATION_FAILED:
            printf("Main table element memory allocation failed.\n") ;
            break ;
        case DATA_IMAGE_ALLOCATION_FAILED:
            printf("Data image table element memory allocation failed.\n") ;
            break;
        }
    if (error != LINE_TOO_LONG) { /* Occured in '.as' and 'am' will not created so it's clear the other files won't be created. */
        printf("The '.ob', '.ent', '.ext' files will not be created. Getting next input file (if exist).\n\n") ;
    }
}
