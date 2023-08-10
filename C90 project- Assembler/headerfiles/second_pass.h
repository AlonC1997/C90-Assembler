#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/* Headears include: */
#include "first_and_second_pass_data.h"
#include "first_pass.h"

/* Functions: */
/*
 * Function: check_for_not_def_entry_label (static)
 * --------------------------------------
 * Checks for any undefined entry labels in the list of defined labels.
 * If an entry label is found that was not defined as a regular label,
 * an error message is generated for each occurrence during the second pass of assembly.
 * -------------------------
 * Parameters:
 *   1: Pointer to head of def_label_table.
 *   2: File name for printing errors. (line number is inside the struct node).
 *   3: A pointer to the entry points file where the label and address will be written.
 * -------------------------
 * Returns:
 *   If all entry label defined at file: TRUE(0).
 *   If there is at least one label that not defined at file: FALSE (0).
 * -------------------------
 * static int check_for_not_def_entry_label(def_label_ptr *, const char*, FILE *) ; 
 */

/*
 * Function: handle_second_pass
 * ---------------------------
 * Performs the second pass of the assembly process to generate the output files: .ob, .ext, and .ent.
 * During the second pass, the function processes the main instruction list and the defined label list
 * to generate the final output files and handle any potential errors.
 * ---------------------------
 * Parameters:
 *   1: A pointer to the output object file (.ob) where the machine code will be written.
 *   2: A pointer to the external references file (.ext) where external label references will be written.
 *   3: A pointer to the entry points file (.ent) where entry label definitions will be written.
 *   4: A pointer to the head of the main instruction list.
 *   5: A pointer to the head of the data image list.
 *   6: A pointer to the head of the defined label list.
 *   7: The current instruction counter (IC) value representing the final address in memory after the first pass.
 *   8: The data counter (DC) value representing the length of the data segment.
 *   9: The name of the input assembly file being processed. It is used in error messages and output file names.
 * ---------------------------
 * Returns:
 *   An integer value indicating the success of the second pass:
 *   - If any error is encountered during the second pass (e.g., undefined entry labels, invalid operands),
 *     the function returns FALSE (0).
 *   - If the second pass completes without errors, the function returns TRUE (1).
 * ---------------------------
 */
int handle_second_pass (FILE *ob_f, FILE *ext_f, FILE *ent_f, main_ptr *main_head, data_image_ptr *data_head, def_label_ptr *def_head, int, int,  const char *file_name) ;


#endif /* SECOND_PASS_H */
