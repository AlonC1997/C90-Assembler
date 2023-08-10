#ifndef FIRST_AND_SECOND_PASS_DATA_H
#define FIRST_AND_SECOND_PASS_DATA_H

/* Headears include: */
#include "first_pass_helpers.h"

/* Struct:*/
/* Defines a pointer type for the struct main_node. */
typedef struct main_node *main_ptr ;

/* Defines a pointer type for the struct def_label_node. */
typedef struct def_label_node *def_label_ptr ;

/* Defines a pointer type for the struct data_image_node.*/
typedef struct data_image_node *data_image_ptr ;

/* Function: */
/*
 * Function: create_data_image_node
 * ----------------------
 * This function creates a new node in the data image table and adds it to the end of the linked list.
 * It assigns the specified values for the address, dc (data count), and flag fields of the new node.
 * The new node is added to the linked list pointed by 'head', and the 'tail' pointer is updated to 
 * point to the last node in the list.
 * ----------------------
 * Parameters:
 *   1: Pointer to the head of the linked list (data image table).
 *   2: Pointer to he tail of the linked list (data image table).
 *   3: The address value to be assigned to the new node.
 *   4: Flag which indicate if Its first or last or both (if there is only one data for this address).
 *   5: The value of the num in 16 Bits, only 12 first bits are working.
 *   6: The name of the file where the label is defined.
 *   7: The line number in the file where the label is defined.
 * ----------------------
 * void.
 * ----------------------
 */ 
void create_data_image_node(data_image_ptr *head , data_image_ptr *tail, int address, int flag, short int num, const char *file_name, int line_number) ;

/*
 * Function: create_main_node
 * -------------------------
 * Creates a new node in the main list and inserts it at the end of the list.
 * If the list is empty, the new node becomes both the first and last element.
 * If the list is not empty, the new node is inserted at the end, and the tail pointer is updated accordingly.
 * If memory allocation fails, an error will be handled, and appropriate action will be taken.
 * -------------------------
 * Parameters:
 *   1:  Pointer to the first element in the main list.
 *   2:  Pointer to the last element in the main list.
 *   3:  Line type.
 *   4:  Address_method value representing the addressing method of the source operand.
 *   5:  Address_method value representing the addressing method of the target operand.
 *   6:  The starting address of the instruction.
 *   7:  The name of the source operand (if applicable).
 *   8:  The name of the target operand (if applicable).
 *   9:  File name is always for sent to print the file name in case of error.
 *   10: Line number in the source file where the instruction is defined.
 *   11: Source length.
 *   12: Target length.
 * -------------------------
 * void.
 * -------------------------
 */
void create_main_node(main_ptr *head, main_ptr *tail, LINE_INDICATION type, Address_method source, Address_method target, int start_address, const char *source_name, const char *target_name, const char *file_name, int line_number, int source_length, int target_length) ;

/*
 * Function: create_def_label_node 
 * ------------------------------
 * Creates a new node in the def_label_table list (for the require table) if the label with the given name does not already exist in the list.
 * The function checks each node in the list to detect possible errors and warnings according to different flag combinations.
 * If a duplicate label is found, appropriate errors and warnings are printed and flags are updated accordingly.
 * The function also handles the scenario where a label is declared in both entry and regular statements, updating the flags for the node accordingly.
 * If memory allocation fails, an error will be handled, and appropriate action will be taken.
 * ------------------------------
 * Parameters:
 *   1: Pointer to the first element in the def_label_table list.
 *   2: The name of the label to be defined.
 *   3: Length of label name to compare.
 *   4: Extern statement flag (TRUE (1) - Declared on an external line, FALSE (0) - Not declared).
 *   5: Entry statement flag (TRUE (1) - Declared on an entry line, FALSE (0) - Not declared).
 *   6: Regular label statement flag (TRUE (1) - Declared in Regular label statement, FALSE (0) - Not declared).
 *   7: The current address (Decimal from 100 to 1023).
 * 7 & 8 parameter are not the same because we need both for checking if label is already defined in some line or just use as parameter.
 *   8: The address where the label is defined (Decimal from 100 to 1023). 
 *   9: The line number in the file where the label is defined.
 *   10: The name of the file where the label is defined.
 *   11: Error flag.
 * ------------------------------
 * Return:
 *   TRUE (1): If success adding. 
 *   FALSE (0): If error found. label wasnt adding.
 * ------------------------------
 */
int create_def_label_node(def_label_ptr *head, const char *def_label_name, int length, int extern_flag, int entry_flag, int regular_flag, int address, int def_line, int line_number, const char *file_name, int *error_flag) ;

/*
 * Function: define_data_image_node_values (static)
 * ----------------------
 *  * Defines values that make the node in the data_image list.
 * ----------------------
 * Parameters:
 *   1: Pointer to a the data image node whose values need to be defined.
 *   2: The address value to be assigned to the data image node.
 *   4: Flag which indicate if Its first or last or both (if there is only one data for this address).
 *   5: The value of the num to defined in 16 Bits, only 12 first bits are working.
 * ----------------------
 * static void define_data_image_node_values(data_image_ptr *h, int, int, short int) ;
 */

/*
 * Function: define_main_node_values (static)
 * ----------------------
 * Defines values that make the node in the main.
 * -------------------------
 * Parameters:
 *   1:  Pointer to first element in main table list.
 *   2:  Line indicate.
 *   3:  Address_method (enum) source - In case the opcode isnt (0-15) so 0 Indicate string line, 00 Indicate data line. (1/3/5) for addressing method 
 *   4:  Address_method target(enum) - In case the opcode isnt (0-15) 0 Indicate string line, 00 Indicate data line. (1/3/5) for addressing method 
 *   5:  line number (in file).
 *   6:  Starting address (100-1023).
 *   7:  Source operand name. NULL if there isnt. If its data line, the line data will store here.
 *   8:  Target opernad name. NULL if there isnt.
 *   9:  Source length.
 *   10: Target length.
 *   11: The name of the file where the labels are defined.
 * -------------------------
 * void.
 * -------------------------
 * static void define_main_node_values(main_ptr *h, LINE_INDICATE, Address_method, Address_method, int, int, const char *,const char *, int, int, const char*) ;
 */

 /*
 * Function: define_def_label_node_values (static)
 * ----------------------
 * Defines values that make the node in the def_label_table list (for the require table).
 * -------------------------
 * Parameters:
 *   1: Pointer to first element in external list.
 *   2: Label name.
 *   3: Length of label name to set.
 *   4: Extern statment flag. (TRUE (1)- Declared on an external line, FALSE(0) - Not declared).
 *   5: Entry statment flag. (TRUE (1)- Declared on an entry line, FALSE(0) - Not declared).
 *   6: Regular label statment flag. (TRUE (1)- Declared in Regular label statment, FALSE(0) - Not declared).
 *   7: The address it defined (Decimal from 100 to 1023).
 *   8: Defined line in file.
 *   9: The name of the file where the label is defined.
 * -------------------------
 * void.
 * -------------------------
 * static void define_def_label_node_values(def_label_ptr *, const char *, int, int, int, int, int, int, const char*) ;
 */

/* Getter and Setter Functions for main_node: */
/*
 * Function: main_node_get_type
 * ----------------------
 * Retrieves the type of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   The LINE_INDICATION type of the main_node.
 * ----------------------
 */
LINE_INDICATION main_node_get_type(const main_ptr) ;

/*
 * Function: main_node_get_source
 * ----------------------
 * Retrieves the source address method of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   The Address_method source address method of the main_node.
 * ----------------------
 */
Address_method main_node_get_source(const main_ptr) ;

/*
 * Function: main_node_get_target
 * ----------------------
 * Retrieves the target address method of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   The Address_method target address method of the main_node.
 * ----------------------
 */
Address_method main_node_get_target(const main_ptr) ;

/*
 * Function: main_node_get_line_number
 * ----------------------
 * Retrieves the line number of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   The line number of the main_node.
 * ----------------------
 */
int main_node_get_line_number(const main_ptr) ;

/*
 * Function: main_node_get_start_address
 * ----------------------
 * Retrieves the start address of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   The start address of the main_node.
 * ----------------------
 */
int main_node_get_start_address(const main_ptr) ;
 
/*
 * Function: main_node_get_source_name
 * ----------------------
 * Retrieves the source name of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   A pointer to the source name of the main_node.
 * ----------------------
 */
const char *main_node_get_source_name(const main_ptr) ;

/*
 * Function: main_node_get_target_name
 * ----------------------
 * Retrieves the target name of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   A pointer to the target name of the main_node.
 * ----------------------
 */
const char *main_node_get_target_name(const main_ptr) ;

/*
 * Function: main_node_get_first_line_code
 * ----------------------
 * Retrieves the first line code of the main_node.
 * ----------------------
 * Parameters:
 *   1: node - A pointer to the main_node.
 * ----------------------
 * Returns:
 *   The first line code of the main_node.
 * ----------------------
 */
short int main_node_get_first_line_code(const main_ptr) ;

/*
 * Function: main_node_set_first_line_code
 * ----------------------
 * Sets the first line code of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 *   2: The first line code to set for the main_node.
 * ----------------------
 * void.
 * ----------------------
 */
void main_node_set_first_line_code(main_ptr, short int) ;

/*
 * Function: main_node_get_second_line_code
 * ----------------------
 * Retrieves the second line code of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   The second line code of the main_node.
 * ----------------------
 */
short int main_node_get_second_line_code(const main_ptr) ;

/*
 * Function: main_node_set_second_line_code
 * ----------------------
 * Sets the second line code of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 *   2: The second line code to set for the main_node.
 * ----------------------
 * void.
 * ----------------------
 */
void main_node_set_second_line_code(main_ptr, short int) ;

/*
 * Function: main_node_get_third_line_code
 * ----------------------
 * Retrieves the third line code of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   The third line code of the main_node.
 * ----------------------
 */
short int main_node_get_third_line_code(const main_ptr) ;

/*
 * Function: main_node_set_third_line_code
 * ----------------------
 * Sets the third line code of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 *   2: The third line code to set for the main_node.
 * ----------------------
 * void.
 * ----------------------
 */
void main_node_set_third_line_code(main_ptr, short int) ;

/*
 * Function: main_node_get_next
 * ----------------------
 * Retrieves the next node pointer of the main_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the main_node.
 * ----------------------
 * Returns:
 *   A pointer to the next main_node.
 * ----------------------
 */
main_ptr main_node_get_next(const main_ptr) ;


/* Getter and Setter Functions for def_label_node: */
/*
 * Function: def_label_node_get_name
 * ----------------------
 * Retrieves the name of the def_label_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the def_label_node.
 * ----------------------
 * Returns:
 *   A pointer to the name of the def_label_node.
 * ----------------------
 */
const char *def_label_node_get_name(def_label_ptr) ;

/*
 * Function: def_label_node_get_extern_flag
 * ----------------------
 * Retrieves the extern flag of the def_label_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the def_label_node.
 * ----------------------
 * Returns:
 *   The extern flag value of the def_label_node.
 * ----------------------
 */
int def_label_node_get_extern_flag(const def_label_ptr) ;

/*
 * Function: def_label_node_get_entry_flag
 * ----------------------
 * Retrieves the entry flag of the def_label_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the def_label_node.
 * ----------------------
 * Returns:
 *   The entry flag value of the def_label_node.
 * ----------------------
 */
int def_label_node_get_entry_flag(const def_label_ptr) ;

/*
 * Function: def_label_node_get_regular_flag
 * ----------------------
 * Retrieves the regular flag of the def_label_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the def_label_node.
 * ----------------------
 * Returns:
 *   The regular flag value of the def_label_node.
 * ----------------------
 */
int def_label_node_get_regular_flag(const def_label_ptr) ;

/*
 * Function:  def_label_node_get_address
 * ----------------------
 * Retrieves the address of the def_label_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the def_label_node.
 * ----------------------
 * Returns:
 *   The address of the def_label_node.
 * ----------------------
 */
int def_label_node_get_address(const def_label_ptr) ;

/*
 * Function: def_label_node_get_def_line
 * ----------------------
 * Retrieves the def_line of the def_label_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the def_label_node.
 * ----------------------
 * Returns:
 *   The def_line value of the def_label_node.
 * ----------------------
 */
int def_label_node_get_def_line(const def_label_ptr) ;

/*
 * Function: def_label_node_get_next
 * ----------------------
 * Retrieves the next node pointer of the def_label_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the def_label_node.
 * ----------------------
 * Returns:
 *   A pointer to the next def_label_node.
 * ----------------------
 */
def_label_ptr def_label_node_get_next(const def_label_ptr) ;


/* Getter Functions for data_image_node: */
/*
 * Function: data_image_node_get_address
 * ----------------------
 * Retrieves the address of the data_image_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the data_image_node.
 * ----------------------
 * Returns:
 *   The address of the data_image_node.
 * ----------------------
 */
int data_image_node_get_address(const data_image_ptr) ;

/*
 * Function: data_image_node_get_flag
 * ----------------------
 * Retrieves the flag value of the data_image_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the data_image_node.
 * ----------------------
 * Returns:
 *   The flag value of the data_image_node.
 * ----------------------
 */
int data_image_node_get_flag(const data_image_ptr) ;

/*
 * Function: data_image_node_get_num
 * ----------------------
 * Retrieves the num value of the data_image_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the data_image_node.
 * ----------------------
 * Returns:
 *   The num value of the data_image_node.
 * ----------------------
 */
short int data_image_node_get_num(const data_image_ptr) ;

/*
 * Function: data_image_node_get_next
 * ----------------------
 * Retrieves the next node pointer of the data_image_node.
 * ----------------------
 * Parameters:
 *   1: A pointer to the data_image_node.
 * ----------------------
 * Returns:
 *   A pointer to the next data_image_node.
 * ----------------------
 */
data_image_ptr data_image_node_get_next(const data_image_ptr) ;

/*
 * Function: malloc_for_main
 * ----------------------
 * Allocating memory to a main item in memory.
 * It checks by itself if the allocation was successful.
 * Prints an error if it was not successful and exits the program (as required in the project if memory allocation is not successful- fatal error).
 * ----------------------
 * Parameters:
 *   1: File name is always for sent to print the file name in case of error.
 *   2: Line number is always for sent to print the line number in case of error.
 * ----------------------
 * Returns:
 *   Pointer to the allocated memory.
 * ----------------------
 */
main_ptr malloc_for_main(const char *, int) ;

/*
 * Function: malloc_for_def_label
 * ----------------------
 * Allocating memory to a def_label item in memory.
 * It checks by itself if the allocation was successful.
 * Prints an error if it was not successful and exits the program (as required in the project if memory allocation is not successful- fatal error).
 * ----------------------
 * Parameters:
 *   1: File name is always for sent to print the file name in case of error.
 *   2: Line number is always for sent to print the line number in case of error.
 * ----------------------
 * Returns:
 *   Pointer to the allocated memory.
 * ----------------------
 */
def_label_ptr malloc_for_def_label(const char *, int) ;

/*
 * Function: malloc_for_data_image
 * ----------------------
 * Allocating memory to a data_image item in memory.
 * It checks by itself if the allocation was successful.
 * Prints an error if it was not successful and exits the program (as required in the project if memory allocation is not successful- fatal error).
 * ----------------------
 * Parameters:
 *   1: File name is always for sent to print the file name in case of error.
 *   2: Line number is always for sent to print the line number in case of error.
 * ----------------------
 * Returns:
 *   Pointer to the allocated memory.
 * ----------------------
 */
data_image_ptr malloc_for_data_image(const char * , int ) ;

/*
 * Function: free_def_label_list
 * ----------------------
 * frees to def_label list allocated memory (for all the list item).
 * -------------------------
 * Parameters:
 *   1: Pointer to the first element of the def_label list.
 * -------------------------
 * void.
 * -------------------------
 */
void free_def_label_list(def_label_ptr *) ;

/*
 * Function: free_main_list
 * ----------------------
 * frees to main list allocated memory (for all the list item).
 * -------------------------
 * Parameters:
 *   1: Pointer to the first element of the main list.
 * -------------------------
 * void.
 * -------------------------
 */
void free_main_list(main_ptr *) ;

/*
 * Function: free_data_list
 * ----------------------
 * frees to data image list allocated memory (for all the list item).
 * -------------------------
 * Parameters:
 *   1: Pointer to the first element of the data image list.
 * -------------------------
 * void.
 * -------------------------
 */
void free_data_list(data_image_ptr *) ;

/*
 * Function: free_all_first_pass_lists
 * ----------------------
 * frees to data image list allocated memory (for all the list item).
 * -------------------------
 * Parameters:
 *   1: Pointer to the first element of the main list.
 *   2: Pointer to the first element of the data image list.
 *   3: Pointer to the first element of the def label list.
 * -------------------------
 * void.
 * -------------------------
 */
void free_all_first_pass_lists(main_ptr *, data_image_ptr *, def_label_ptr *) ;


#endif /* FIRST_AND_SECOND_PASS_DATA_H */
