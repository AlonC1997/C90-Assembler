#ifndef MACRO_DATA_H
#define MACRO_DATA_H

/* Struct:*/
/* Defines a pointer type for the struct macro_node. */
typedef struct macro_node *macro_ptr ;

/* Function: */
/*
 * Function: malloc_for_macro
 * ----------------------
 * Allocating memory to a macro item in memory.
 * It checks by itself if the allocation was successful.
 * Prints an error if it was not successful and exits the program (as required in the project if memory allocation is not successful- fatal error).
 * -------------------------
 * Parameters:
 *   1: File name is always for sent to print the file name in case of error.
 *   2: Line number is always for sent to print the line number in case of error.
 * -------------------------
 * Returns:
 *   Pointer to the allocated memory.
 * -------------------------
 */
macro_ptr malloc_for_macro(const char *, int ) ;

/*
 * Function: free_macro_list
 * ----------------------
 * frees to macro list allocated memory (for all the list item).
 * -------------------------
 * Parameters:
 *   1: Pointer to the first element of the macros list.
 * -------------------------
 * void.
 * -------------------------
 */
void free_macro_list(macro_ptr *h) ;  

/*
 * Function: create_macro_node (static)
 * ----------------------
 * Creates a new node in the macro linked list.
 * -------------------------
 * Parameters:
 *   1: Pointer to first element in connected list of macros in '.as' file.
 *   2: Pointer to last element in connected list of macros in '.as' file.
 *   3: Unchangeable flag of def macro detecting to understand if its first line of macro definition, middle lines or endmcro line.
 *   4: Unchangeable flag of end macro detecting to differentiate between case 2 and 3 in the function.
 *   5: Pointer to first non white char of line.
 *   6: File name is always for sent to print the file name in case of error.
 *   7: Line number is always sent to print the line number in case of error. 
 * -------------------------
 * void.
 * -------------------------
 */
void create_macro_node(macro_ptr *head, macro_ptr *tail, int macro_def_flag, int macro_end_flag, char * line_p, const char *file_name, int line_number) ;
 
/*
 * Function: define_macro_values (static)
 * ----------------------
 * Defines 3 values that make up a macro (macro_content, macro_def_flag, macro_end_flag).
 * pointer to next element will be defined separately and also the name depending on other things.
 * -------------------------
 * Parameters:
 *   1: Pointer to first element in connected list of macros in '.as' file.
 *   2: macro_content string value to define.
 *   3: macro def_flag int value to define.
 *   4: macro end_flag int value to define.
 *   5: File name is always for sent to print the file name in case of error.
 *   6: Line number is always sent to print the line number in case of error. 
 * -------------------------
 * void.
 * -------------------------
 * static void define_macro_values(macro_ptr *, char *, int, int, const char*, int) ;
 */

/* Getters and Setters: */
/*
 * Function: macro_node_get_def_flag
 * -------------------------------
 * Retrieves the def_flag value of a macro_node.
 * -------------------------------
 * Parameters:
 *   1: A pointer to the macro_node.
 * -------------------------------
 * Returns:
 *   The def_flag value of the macro_node.
 * -------------------------------
 */
int macro_node_get_def_flag(const macro_ptr) ;

/*
 * Function: macro_node_get_end_flag
 * -------------------------------
 * Retrieves the end_flag value of a macro_node.
 * -------------------------------
 * Parameters:
 *   1: A pointer to the macro_node.
 * -------------------------------
 * Returns:
 *   The end_flag value of the macro_node.
 * -------------------------------
 */
int macro_node_get_end_flag(const macro_ptr node) ;

/*
 * Function: macro_node_get_macro_name
 * -------------------------------
 * Retrieves the macro_name string of a macro_node.
 * -------------------------------
 * Parameters:
 *   1: A pointer to the macro_node.
 * -------------------------------
 * Returns:
 *   A pointer to the macro_name string of the macro_node.
 * -------------------------------
 */
const char *macro_node_get_macro_name(const macro_ptr node) ;

/*
 * Function: macro_node_get_macro_content
 * -------------------------------
 * Retrieves the macro_content string of a macro_node.
 * -------------------------------
 * Parameters:
 *   1: A pointer to the macro_node.
 * -------------------------------
 * Returns:
 *   A pointer to the macro_content string of the macro_node.
 * -------------------------------
 */
const char *macro_node_get_macro_content(const macro_ptr) ;

/*
 * Function: macro_node_get_next
 * -------------------------------
 * Retrieves the next pointer of a macro_node.
 * -------------------------------
 * Parameters:
 *   1: A pointer to the macro_node.
 * -------------------------------
 * Returns:
 *   The next pointer of the macro_node.
 * -------------------------------
 */
macro_ptr macro_node_get_next(const macro_ptr) ;


#endif /* MACRO_DATA_H */