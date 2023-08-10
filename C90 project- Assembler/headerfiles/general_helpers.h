#ifndef GENERAL_HELPERS_H
#define GENERAL_HELPERS_H

/* Macros definition */
/* Some of the macros have the same value for different names, this is for the code to be more readable and clear. */
#define MAX_LINE 81 /* One Extra for the NULL character. */
#define ILEGAL -999 /* Using for TRUE/FALSE function which we need both true and false being return. */
#define DEFAULT_VALUE 0 
#define FALSE 0
#define TRUE 1
#define NUM_OF_FUNCTIONS 16 
#define NUM_OF_DIRECTIVE 4 
#define COPY_AS_IS 1
#define DONT_COPY 2
#define MACRO_NAME 3
#define DIRECTIVE_MAX_SIZE 8 /* Include another one for auto pad with '\0'. */
#define FUNC_NAME_MAX_SIZE 5 /* Include another one for auto pad with '\0'. */
#define MCRO_DEF_LENGTH 5 /* Include another one for auto pad with '\0'. */
#define ENDMCRO_LENGTH 8 /* Include another one for auto pad with '\0'. */
#define MAX_MEMORY_CELL 1023  /* There is 1024 cells (0-1023), first 100 (0-99) used for computer and not fot us. Our cells are 100-1023. */
#define FIRST_MEMORY_CELL 100
#define MIN_ARGC 2
#define MAX_FILE_NAME 20 /* Assume. */
#define EXTENSION_LENGTH 4 /* '.as' but including also extra space for '\0'. */
#define NUM_OF_REGISTAR 8 
#define REGISTAR_MAX_SIZE 3 /* Include another one for auto pad with '\0'. */
#define LABEL_MAX_LENGTH 32 /* Include another one for auto pad with '\0'. */ 
#define AM_EXTENTSION ".am"
#define AS_EXTENTSION ".as"
#define OB_EXTENTSION ".ob"
#define EXT_EXTENTSION ".ext"
#define ENT_EXTENTSION ".ent"

/* Functions: */
/*
 * Function: int consecutive_white_characters
 * -------------------------
 * Count consecutive white char (not include newline char '\n').
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 * -------------------------
 * Returns:
 *   Number of consecutive white chars.
 * -------------------------
 */
int consecutive_white_characters(const char *) ;

/*
 * Function: int consecutive_non_white_characters
 * -------------------------
 * Count consecutive non white char (not include newline char '\n').
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 * -------------------------
 * Returns:
 *   Number of consecutive non white chars.
 * -------------------------
 */
int consecutive_non_white_characters(const char *) ;

/*
 * Function: is_it_empty_line
 * -------------------------
 * Checks whether from pointer the line is empty (only spaces/tabs) from the pointer it received to the end (not include newline char '\n').
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 * -------------------------
 * Returns:
 *   TRUE(1)- if it is empty line.
 *   FALSE(0)- if it is not empty line.
 * -------------------------
 */
int is_it_empty_line(const char *) ;

/*
 * Function: is_it_comment_line
 * -------------------------
 * Checks whether is this a comment line.
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 * -------------------------
 * Returns:
 *   TRUE(1)- if it is comment line.
 *   FALSE(0)- if it is not comment line.
 * -------------------------
 */
int is_it_comment_char(const char *) ;

/*
 * Function: is_it_function_name
 * -------------------------
 * Checks if a sequence of characters is a function name.
 * Also use for returning the exact function number. (That's the reason we dont use False when returning cause 0 is function index also).
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 *   2: Number of consecutive chars to compare with.
 * -------------------------
 * Returns:
 *   >= 0  - if it is function name.
 *   -1 - if it is not function name.
 * -------------------------
 */
int is_it_function_name(const char *, int) ;

/*
 * Function: is_it_directive_name
 * -------------------------
 * Checks if a sequence of characters is a directive name.
 * Also use for returning the exact directive number. (That's the reason we dont use False when returning cause 0 is directive index also).
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 *   2: Number of consecutive chars to compare with.
 * -------------------------
 * Returns:
 *   >= 0- if it is directive name. 
 *   -1 - if it is not directive name.
 * -------------------------
 */
int is_it_directive_name(const char *, int ) ;

/*
 * Function: is_it_registar_name
 * -------------------------
 * Checks if a sequence of characters is a registar name.
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 *   2: Number of consecutive chars to compare with.
 *   3: Error_flag to update.
 * -------------------------
 * Returns:
 *   register index (0-7)- if it is register name.
 *   ILEGAL- if it is not registar name.
 */
int is_it_register_name(const char *, int, int *) ;

/*
 * Function: malloc_for_char
 * ----------------------
 * Allocating memory to a string in memory.
 * It checks by itself if the allocation was successful.
 * Prints an error if it was not successful and exits the program (as required in the project if memory allocation is not successful- fatal error).
 * -------------------------
 * Parameters:
 *   1: Number of chars cells to allocate.
 *   2: File name is always for sent to print the file name in case of error.
 *   3: Line number is always for sent to print the line number in case of error.
 * -------------------------
 * Returns:
 *   Pointer to the first char in the allocated memory.
 * -------------------------
 */
char * malloc_for_char(int, const char *, int) ;


#endif /* GENERAL_HELPERS_H */
