#ifndef ERROR_H
#define ERROR_H

/* Enumerations: */
/*
 * Enumerations: Error and warning.
 * -------------------------
 * These enumerations define various error and warning indications that can occur during different stages of assembly processing. 
 * -------------------------
 */

/* Warnings (for all assembly processing). */
typedef enum {
    LABEL_BEFORE_ENTRY,
    LABEL_BEFORE_EXTERN,
    STRING_IS_EMPTY,
    DATA_IS_EMPTY,
    LABEL_DEFINED_NOTHING,
    EXTERN_STATEMENT_TWICE,
    ENTRY_STATEMENT_TWICE
} Warning ;

/* File errors. */
typedef enum {
    FILE_OPEN_ERROR,
    READING_ERROR, 
    DELETE_FILE_ERROR,
    ERROR_INDICATOR_IS_SET
} FileError ;

/* Macro pass errors. */
typedef enum {
    END_MACRO_WITHOUT_DEF,
    NO_DEF_FOR_MACRO, 
    MACRO_DEF_WITHOUT_NAME,
    NAME_OF_MACRO_LIKE_DIRECTIVE,
    NAME_OF_MACRO_LIKE_FUNCTION,
    NAME_OF_MACRO_LIKE_REGISTER,
    MACRO_NAME_LINE_ISNT_EMPTY_AFTER_NAME,
    DEF_LINE_ISNT_EMPTY_AFTER_NAME,
    END_MACRO_LINE_ISNT_EMPTY_AFTER_ENDMCRO,
    MACRO_CANT_REDEFINED
} MacroPassError ;

/* First pass general errors. */
typedef enum {
    COMMENT_LINE_ILLEGAL_SPACES,
    ADDITIONAL_UNRELATED_TEXT,
    TWO_CONSECUTIVE_COMMAS, 
    ILEGAL_COMMA,
    MISSING_COMMA,
    MISSING_SOURCE_PARAMTER,
    MISSING_TARGET_PARAMTER,
    A_NUMBER_SHOULD_APPEAR_AFTER_PLUS_OR_MINUS,
    UNAPROPIATE_SOURCE_PARAMETER,
    UNAPROPIATE_TARGET_PARAMETER,
    ONLY_INTEGERS_NUMBERS_ARE_ACCEPT,
    NAME_OF_REGISTAR_IS_MISSING_AFTER_ATSIGN,
    ILEGAL_REGISTER_NAME
} FirstPassGeneralError ;

/* First pass label errors. */
typedef enum {
    LABEL_NAME_SHOULD_START_WITH_LETTER,
    LABEL_NAME_CONTAIN_ONLY_LETTERS_AND_NUMBERS,
    LABEL_MAX_LENGTH_IS_31,
    LABEL_NAME_CANT_BE_AS_OPCODE_NAME,
    LABEL_NAME_CANT_BE_AS_DIRECTIVE_NAME,
    LABEL_NAME_CANT_BE_AS_REGISTAR_NAME,
    LABEL_NAME_CANT_REDEFINED,
    LABEL_CANT_BE_BOTH_EXTERNAL_AND_ENTRY,
    LABEL_CANT_DEFINED_LABEL,
    TWO_CONSECUTIVE_COLON,
    MISSING_COLON
} FirstPassLabelError ;

/* First pass directive errors. */
typedef enum {
    TWO_CONSECUTIVE_DOT,
    EXTERNAL_LABEL_CANT_BE_REDEFINED_IN_A_FILE,
    MISSING_LABEL_AFTER_COMMA_OR_ILEGAL_COMMA_AT_END,
    EXTERN_IS_EMPTY,
    ENTRY_CONTAIN_ONE_LABEL_FOLLOWED_BY_NOTHING,
    ENTRY_IS_EMPTY,
    DATA_LINE_OPERANDS_ARE_INTEGER,
    MISSING_NUM_AFTER_COMMA_OR_ILEGAL_COMMA_AT_END,
    STRING_SHOULD_START_WITH_PAIRS_OF_QUOTATION_MARKS,
    STRING_SHOULD_END_WITH_PAIRS_OF_QUOTATION_MARKS,
    STRING_DIRECTIVE_CONTAIN_ONE_STRING_FOLLOWED_BY_NOTHING
} FirstPassDirectiveError ;

/* Second pass errors. */
typedef enum {
    NO_SUCH_LABEL_DEFINED,
    ENTRY_LABEL_WASNT_DEFINED_AT_FILE
} SecondPassError ;

/* Memory error indications. */
typedef enum {
    MACRO_ALLOCATION_FAILED,
    DEF_LABEL_ALLOCATION_FAILED,
    USED_LABEL_ALLOCATION_FAILED,
    MAIN_ALLOCATION_FAILED,
    DATA_IMAGE_ALLOCATION_FAILED,
    CHAR_ARRAY_ALLOCATION_FAILED,
    LINE_TOO_LONG,
    MAXIMUM_MEMORY_EXCEEDED,
    MAX_REGISTER,
    MAX_DATA_INTEGER,
    MAX_OPERAND_INTEGER
} MemoryError;

/* Functions: */
/*
 * Function: warning
 * -------------------------
 * Prints warnings.
 * -------------------------
 * Parameters:
 *   1: Warning to print.
 *   2: File name.
 *   3: Line number (starting from 1).
 * -------------------------
 * void.
 * -------------------------
 */
void warning(Warning, const char *, int) ;

/*
 * Function: file_error
 * ----------------------
 * Prints file handling error
 * -------------------------
 * Parameters:
 *   1: Error to print.
 *   2: File name.
 *   3: Line number (starting from 1).
 * ------------------------- 
 * void.
 * -------------------------
 */
void file_error(FileError, const char *, int) ;

/*
 * Function: macro_pass_error
 * -------------------------
 * Prints macro pass error
 * -------------------------
 * Parameters:
 *   1: Error to print.
 *   2: File name.
 *   3: Line number (starting from 1).
 * -------------------------
 * void.
 * -------------------------
 */
void macro_pass_error(MacroPassError, const char *, int) ;

/*
 * Function: first_pass_general_error
 * -------------------------
 * Prints first pass general errors.
 * Also printing 2 errors for the case of register error.
 * -------------------------
 * Parameters:
 *   1: Error to print.
 *   2: File name.
 *   3: Line number (starting from 1).
 * -------------------------
 * void.
 * -------------------------
 */
void first_pass_general_error(FirstPassGeneralError, const char *, int) ;

/*
 * Function: first_pass_label_error
 * -------------------------
 * Prints first pass label errors.
 * -------------------------
 * Parameters:
 *   1: Error to print.
 *   2: File name.
 *   3: Line number (starting from 1).
 * -------------------------
 * void.
 * -------------------------
 */
void first_pass_label_error(FirstPassLabelError, const char *, int) ;

/*
 * Function: first_pass_directive_error
 * -------------------------
 * Prints first pass directive errors.
 * -------------------------
 * Parameters:
 *   1: Error to print.
 *   2: File name.
 *   3: Line number (starting from 1).
 * -------------------------
 * void.
 * -------------------------
 */
void first_pass_directive_error(FirstPassDirectiveError, const char *, int) ;

/*
 * Function: second_pass_error
 * -------------------------
 * Prints second pass error.
 * -------------------------
 * Parameters:
 *   1: Error to print.
 *   2: File name.
 *   3: Line number (starting from 1).
 * -------------------------
 * void.
 * -------------------------
 */
void second_pass_error(SecondPassError, const char *, int) ;

/*
 * Function: memory_error
 * -------------------------
 * Prints memory_error.
 * -------------------------
 * Parameters:
 *   1: Error to print.
 *   2: File name.
 *   3: Line number (starting from 1).
 * -------------------------
 * void.
 * -------------------------
 */
void memory_error(MemoryError, const char *, int) ;


#endif /* ERROR_H */
