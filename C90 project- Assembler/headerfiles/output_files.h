#ifndef OUTPUT_FILES_H
#define OUTPUT_FILES_H

/* Headears include: */
#include "first_and_second_pass_data.h"
#include "first_pass.h"
#include "macro_data.h"

/* Functions: */
/*
 * Function: spread_am_file_line
 * -------------------------
 * Spreading '.am' file line.
 * -------------------------
 * Parameters:
 *   1: Which case to do.
 *   2: Pointer to the '.am' file to write on it.
 *   3: Pointer to the macros list.
 *   4: Line to spread (or macro name).
 *   5: Pointer to the macro_def_flag (use in handle_macro.c) to change it in case of macro name.
 *   6: File name is always sent to print the file name in case of error (here the use is when allocating memory for char*- temp_name).
 *   7: Line number is always sent to print the line number in case of error (here the use is when allocating memory for char*- temp_name).
 * -------------------------
 * void.
 * -------------------------
 */
void spread_am_file_line(int, FILE *, macro_ptr *, char *, int *, const char *, int) ;

/*
 * Function: first12_bits_to_base64
 * -------------------------
 * Function to convert the first 12 bits of a 16-bit short int to Base64 encoding and print it on given file.
 * -------------------------
 * Parameters:
 *   1: short int (16 bits).
 *   2: Pointer to file to print on it.
 * -------------------------
 * void.
 * -------------------------
 */
 void first12_bits_to_base64(short int , FILE *) ; 

/*
 * Function: print_ob_title
 * ------------------------
 * Writes the instruction counter (IC) and data counter (DC) on the output object (.ob) title file.
 * -------------------------
 * Parameters:
 *   1: A pointer to the output object file.
 *   2: The instruction counter value.
 *   3: The data counter value.
 * -------------------------
 * void.
 * -------------------------
 */
void print_ob_title(FILE *, int, int) ;

/*
 * Function: print_data
 * -------------------------
 * Writes data values *(in BASE64) (string/data) to the output object (.ob) file for a given address.
 * -------------------------
 * Parameters:
 *   1: A pointer to the output object file.
 *   2: A pointer to the linked list head for data image entries.
 *   3: The address associated with the data entries to be written.
 * -------------------------
 * void.
 * -------------------------
 */
void print_data(FILE *, data_image_ptr *, int) ;

/*
 * Function: check_and_print_labels
 * -------------------------
 * Checks if a label (identified by 'label_name') is defined in the list of defined labels (linked list).
 * If the label is found, its associated address and information are printed to the output object file (.ob).
 * If the label is an external label, its name and the current address (using address) are printed to the external references file (.ext). 
 * If the label is not found, an error is printed.
 * -------------------------
 * Parameters:
 *   1: A pointer to the output object file (.ob) where the label information will be written.
 *   2: A pointer to the external references file (.ext) where external label information will be written.
 *   3: The current address in memory corresponding to the label reference.
 *   4: A pointer to a pointer of the head of the defined label list (linked list).
 *   5: The name of the label being searched for in the defined label list.
 *   6: The length of the label name (number of characters) to compare with in the list.
 *   7: The name of the input assembly file being processed.
 *   8: The line number in the input assembly file where the label is referenced.
 *   9: Flag to indicate if its first line of this file - helping us avoid extra white line in the end of this file.
 * -------------------------
 * Return:
 *   TRUE(1) - Label found and printed.
 *   FALSE(0) - Label was not found.
 * -------------------------
 */
 int check_and_print_labels(FILE *, FILE *, int, def_label_ptr *, const char *, int, const char*, int, int*) ;

/*
 * Function: print_label_on_ent
 * -------------------------
 * Writes a label and its associated decimal address to the entry file (.ent).
 * -------------------------
 * Parameters:
 *   1: A pointer to the entry points file where the label and address will be written.
 *   2: The name of the label being defined as an entry point.
 *   3: The decimal address at which the label is defined as an entry point.
 *   4: Flag to indicate if its first line of this file - helping us avoid extra white line in the end of this file.
 * -------------------------
 * void.
 * -------------------------
 */ 
 void print_label_on_ent(FILE *, const char *, int, int) ;
 
/*
 * Function: six_bits_to_base64_value (static)
 * -------------------------
 * Function to convert a 6-bit value to the corresponding Base64 character.
 * This function made for first12_bits_to_base64 function.
 * -------------------------
 * Parameters:
 *   1: 6-bit value as an input (an unsigned char with a range of 0 to 63).
 * -------------------------
 * Returns:
 *   Corresponding Base64 character for that value (the parameter).
 * -------------------------
 * static char six_bits_to_base64_value(unsigned char value) ;
 */

/*
 * Function: print_label_on_ob (static)
 * ---------------------------
 * Writes the label's information into the output object (.ob) file. This includes its address and the appropriate ARE (Absolute, External, or Relocatable) type.
 * -------------------------
 * Parameters:
 *   1: A pointer to the output object file.
 *   2: The address of the label.
 *   3: The ARE type (Absolute, External, or Relocatable) associated with the label.
 * -------------------------
 * void.
 * -------------------------
 * static void print_label_on_ob(FILE *, int, ARE) ;
 */

/*
 * Function: print_label_on_ext (static)
 * -------------------------
 * Writes a label and its associated decimal address to the external file (.ext).
 * -------------------------
 * Parameters:
 *   1: A pointer to the external references file where the label and address will be written.
 *   2: The name of the label being referenced externally.
 *   3: The decimal address at which the label is being referenced.
 *   4: Flag to indicate if its first line of this file - helping us avoid extra white line in the end of this file.
 * -------------------------
 * void.
 * -------------------------
 * static void print_label_on_ext(FILE *, const char *, int, int) ;
 */
 

#endif /* OUTPUT_FILES_H */
