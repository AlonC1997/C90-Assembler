#ifndef FIRST_PASS_H
#define FIRST_PASS_H

/* Headears include: */
#include <stdio.h>

/* Functions: */
/*
 * Function: handle_first_pass
 * ---------------------------
 * Performs the first pass of the assembly code. It reads the input assembly file line by line
 * and processes each line to build the necessary data structures for the second pass.
 * This function handles labels, opcode statements, data statements, entry statements, and extern statements.
 * Calling second pass if everything worked well at first pass and no errors was indicate.
 * ---------------------------
 * Parameters:
 *   1: The name of the input assembly file being processed.
 *   2: A file pointer to the input assembly file.
 *   3: A file pointer to the output object file.
 *   4: A file pointer to the output extern file.
 *   5: A file pointer to the output entry file.
 * ---------------------------
 * Returns:
 *   TRUE (1): If the first pass (and second pass) completes successfully without any errors.
 *   FALSE (0): If an error occurs during the first pass and it cannot proceed further.
 * ---------------------------
 */
int handle_first_pass(const char * file_name, FILE *process_f, FILE *ob_f, FILE *ext_f, FILE *ent_f) ;

/*
 * Function: operands_line_code (static)
 * ----------------------
 * Making short int (16 bits) code with '|' bitwise operator for 3 parametrs.
 * Also this function doing shift for the bits so they will be at the right position.
 * Also this function work well for lines without soure operand- 0 will send as source operand.
 * 0-1 bits are represent (ARE). - In this function will always be 0. (00 for the 2 first bits).
 * 2-6 bits are represent target registar.
 * 7-11 bits are represent source registar.
 * 12-15 always 0 (0000) easier to work with short int (16 bits).
 * -------------------------
 * Parameters:
 *   1: short int opcode.
 *   2: short int of source addressing method.
 *   3: short int of target addressing method.
 * -------------------------
 * Returns:
 *   short int (result of opcode|source|target|are). 
 *   4 last bits (12-15 indexes) will be zero. Because we need only first 12 bits.
 * -------------------------
 * static short int operands_line_code(short int, short int, short int) ; 
 */

/*
 * Function: registar_operands (static)
 * ----------------------
 * Making short int (16 bits) code with '|' bitwise operator for 2 registar parametrs.
 * Also work well for one register (source will be 0).
 * 0-1 bits are represent 00 (Absoulte).
 * 2-6 bits are represent target registar.
 * 7-11 bits are represent source registar.
 * -------------------------
 * Parameters:
 *   1: short int source_registar
 *   2: short int target_registar
 * -------------------------
 * Returns:
 *   short int (result of source_registar|target_registar). 
 *   4 last bits (12-15 indexes) will be zero. Because we need only first 12 bits.
 * ---------------------------
 * static short int register_operands (short int, short int) ;
 */

/*
 * Function: is_it_exist_file_defined_label (static)
 * ---------------------------
 * Checks if a label with the given name already exists in the label table (but no entry or regular).
 * It searches for a label with the same name as the substring of the specified length (`consecutive_chars`) starting from the pointer `p`.
 * ---------------------------
 * Parameters:
 *   1: Pointer to the name of the label to be checked.
 *   2: The number of consecutive characters to compare when searching for the label.
 *   3: Pointer to the head of the label table (linked list).
 * ---------------------------
 * Returns:
 *   If label exists: TRUE (1).
 *   If label does not exist: FALSE (0).
 * --------------------------- 
 * static int is_it_exist_file_defined_label(const char *, int, def_label_ptr *) ;
 */

/*
 * Function: handle_data_line (static)
 * ------------------------------
 * Building data_image list. (also if valid building main node).
 * Using this function after check if '.data' statment is valid. 
 * This function checks whether a given data line in the assembly code is legal or not.
 * It analyzes the data line starting from the specified pointer `p` until the end of the line ('\0').
 * The function performs the following checks:
 *   - Checks if the start of the data line is legal. It allows either a numerical value or a math sign ('+' or '-').
 *   - Checks if there are consecutive white characters after the initial data line statement ('.data') until the first non-white character.
 *   - Parses the data line and checks if each operand is a legal integer value within the allowed range (DATA_NUM).
 *   - Ensures that the data line is either empty after the last valid operand or has another valid operand separated by a comma.
 * -------------------------
 * Parameters:
 *   1:  Pointer to the beginning of the data line to be checked.
 *   2:  Pointer to an integer variable that is set to TRUE if an error is encountered during the analysis.
 *   3:  The name of the source file where the data line is located. Used for error reporting.
 *   4:  The line number in the source file where the data line is located. Used for error reporting.
 *   5:  Pointer to the head of the linked list (main table).
 *   6:  Pointer to the tail of the linked list (main table).
 *   7:  Pointer to the head of the linked list (data image table).
 *   8:  Pointer to the tail of the linked list (data image table).
 *   9:  The address value to be assigned to the new node.
 * -------------------------
 * Returns:
 *   If valid: How many numbers.
 *   If invalid: 0 indicate an error.
 * -------------------------
 * static int handle_data_line(const char*, int*, const char*, int, main_ptr *, main_ptr *, data_image_ptr *, data_image_ptr *, short int) ;
 */

/*
 * Function: handle_string_line (static)
 * ----------------------
 * Building data_image list (also if valid building main node).
 * Using this function after check if '.string' statment is valid. 
 * This function checks if a string is valid according to the following rules:
 * 1) Must start with a double quote (").
 * 2) Must end with a double quote (").
 * 3) After the closing double quote, there are only white characters until the end of the line.
 * 4) Between the opening and closing double quotes, any type of character is allowed, including white characters.
 * If the string is valid, the function, building main and data image new nodes and returns its length+1 (excluding both double quotes but include one for null character). 
 * If it's not valid, it returns a special value (ILEGAL) and prints the appropriate error message. 
 * -------------------------
 * Parameters:
 *   1:  Pointer to the beginning of the string to check.
 *   2:  Pointer to an error flag that is updated if an error is found.
 *   3:  The name of the file being processed (used for error messages).
 *   4:  The line number in the file being processed (used for error messages).
 *   5:  Pointer to the head of the linked list (main table).
 *   6:  Pointer to the tail of the linked list (main table).
 *   7:  Pointer to the head of the linked list (data image table).
 *   8:  Pointer to the tail of the linked list (data image table).
 *   9:  The address value to be assigned to the new node.
 * -------------------------
 * Returns:
 *   If valid: Length of the string+1 (excluding both double quotes but include one for null character).
 *   If invalid: 0 indicate an error.
 * -------------------------
 * static int handle_string_line(const char *, int *, const char*, int, main_ptr *, main_ptr *, data_image_ptr *, data_image_ptr *, short int) ;
 */
 
 /*
 * Function: handle_extern_line (static)
 * ---------------------------
 * Using this function after check if '.extern' statment is valid. 
 * Handles the processing of an '.extern' statement during the first pass of the assembler.
 * It checks if the statement is valid, extracts the label, and creates a new entry in the 
 * defined label table for the external label.
 * ---------------------------
 * Parameters:
 *   1: Pointer to the first char of the string to check (after extern statment).
 *   2: Pointer to an error flag, updated if an error occurs during processing.
 *   3: The name of the source file being processed, used for error reporting.
 *   4: The line number in the source file being processed, used for error reporting.
 *   5: Pointer to the head of the defined label table, where the new entry will be added.
 *   6: The current address in memory where the '.extern' statement is located.
 * ---------------------------
 * void.
 * ---------------------------
 * static void handle_extern_line (const char *, int *, const char*, int, def_label_ptr *, int) ;
 */

/*
 * Function: handle_entry_line (static)
 * ---------------------------
 * Using this function after check if '.entry' statment is valid. 
 * Handles the processing of an '.entry' statement during the first pass of the assembler.
 * It checks if the statement is valid, extracts the label, and creates a new entry in the 
 * defined label table for the entry label.
 * ---------------------------
 * Note: This function assumes that '.entry' has already been recognized as a valid statement.
 * ---------------------------
 * Parameters:
 *   1: Pointer to the first char of the string to check (after entry statment).
 *   2: Pointer to an error flag, updated if an error occurs during processing.
 *   3: The name of the source file being processed, used for error reporting.
 *   4: The line number in the source file being processed, used for error reporting.
 *   5: Pointer to the head of the defined label table, where the new entry will be added.
 *   6: The current address in memory where the '.entry' statement is located.
 * ---------------------------
 * void.
 * ---------------------------
 * static void handle_entry_line (const char *, int *, const char*, int, def_label_ptr *, int) ; 
 */    

/*
 * Function: handle_opcode_without_operands (static)
 * ---------------------------
 * Handles the processing of an opcode line that does not have any operands during the first pass of the assembler.
 * It checks if the statement is valid and generates the machine code for the opcode.
 * This function is used for opcodes like 'rts' and 'stop'.
 * ---------------------------
 * Parameters:
 *   1: The LINE_INDICATION indicating the type of opcode being processed (e.g., RTS, STOP).
 *   2: Pointer to the head of the main list (main program's machine code).
 *   3: Pointer to the tail of the main list.
 *   4: Pointer to the input string representing the opcode line.
 *   5: The current address in memory where the opcode statement is located.
 *   6: Name of the source file currently being processed, used for error reporting.
 *   7: Line number of the current line in the source file, used for error reporting.
 *   8: Pointer to an error flag to update in case of errors.
 * ---------------------------
 * Returns:
 *   - 1: If the opcode line is valid and successfully processed (occupies one memory cell).
 *   - 0: If any errors occur during processing.
 * ---------------------------
 * static int handle_opcode_without_operands(LINE_INDICATION, main_ptr *, main_ptr *, const char*, int , const char *, int , int *) ;
 */

/*
 * Function: handle_opcode_with_one_operand (static)
 * ---------------------------
 * Handles the processing of a 'prn' (A small change in the target operand) / 'jsr' / 'red'  / 'bne'  / 'jmp'  / 'dec'  / 'inc'  / 'not'  / 'clr'  opcodes line during the first pass of the assembler.
 * It checks if the statement is valid and processes its operands.
 * ---------------------------
 * Parameters:
 *   1: LINE_INDICATION type (opcode type).
 *   2: Pointer to the head of the main list (main program's machine code).
 *   3: Pointer to the tail of the main list.
 *   4: Pointer to the first char of the string representing the  line.
 *   5: The current address in memory where the statement is located.
 *   6: Name of the source file currently being processed, used for error reporting.
 *   7: Line number of the current line in the source file, used for error reporting.
 *   8: Pointer to an error flag to update in case of errors.
 * ---------------------------
 * Returns:
 *   - 2: If the '.prn' instruction is valid and successfully processed (occupies two memory cells).
 *   - 0: If any errors occur during processing, if the '.prn' instruction is invalid, or if there are additional
 *        unrelated characters or operands after the operand in the line.
 * ---------------------------
 * static int handle_opcode_with_one_operand (LINE_INDICATION, main_ptr *, main_ptr *, const char*, int, const char *, int , int*) ;
 */

/*
 * Function: handle_opcode_with_two_operand (static)
 * ---------------------------
 * Handles the processing of a 'cmp'(A small change in the target operand) / 'mov' / 'add'  / 'sub' / 'lea' (source is only direct label address) opcodes. 
 * It checks if the statement is valid, processes its operands, and generates machine code accordingly.
 * ---------------------------
 * Parameters:
 *   1:  LINE_INDICATION type (opcode type).
 *   2:  Pointer to the head of the main list.
 *   3:  Pointer to the tail of the main list.
 *   4:  Pointer to the first char of the string representing the  line.
 *   5:  The current address in memory where the statement is located.
 *   6:  Name of the source file currently being processed, used for error reporting.
 *   7:  Line number of the current line in the source file, used for error reporting.
 *   8:  Pointer to an error flag to update in case of errors.
 * ---------------------------
 * Returns:
 *   - 3: If the line is valid and successfully processed (occupies three memory cells).
 *   - 2: If the line is valid and successfully processed (occupies two memory cells).
 *   - 0: If any errors occur during processing.
 * ---------------------------
 * static int handle_opcode_with_two_operand(LINE_INDICATION, main_ptr *, main_ptr *, const char*, int, const char *, int, int *) ;
 */

/*
 * Function: handle_first_pass_line (static)
 * ---------------------------
 * Processes an individual line during the first pass of the assembly code.
 * Determines the type of statement (label, opcode, data, entry, extern, string) and handles each case accordingly.
 * Updates the relevant data structures such as the main program list, data image list, and defined label table and used label table.
 * ---------------------------
 * Parameters:
 *   1:  A pointer to the current address in memory. It will be updated during the processing of the line.
 *   2:  A pointer to the current instruction counter. It will be updated during the processing of the line.
 *   3:  A pointer to the current data counter. It will be updated during the processing of the line.
 *   4:  A pointer to the current line being processed.
 *   5:  A Pointer to the head and tail of the main program list.
 *   6:  A Pointer to the tail and tail of the main program list. 
 *   7:  A Pointer to the head and head of the data image list.
 *   8:  A Pointer to the head and tail of the data image list.
 *   9:  A pointer to the head of the defined label table.
 *   10: A pointer to the flag indicating whether any errors have been encountered during the first pass. It will be updated if an error is found.
 *   11: The name of the input assembly ('.am') file being processed.
 *   12: The line number of the current line being processed.
 * ---------------------------
 * void.
 * ---------------------------
 * void static handle_first_pass_line(int *, int *, int *, char *, main_ptr *, main_ptr *, data_image_ptr *, data_image_ptr *, def_label_ptr *, int *, const char *, int) ;
 */
 
/*
 * Function: handle_def_label_line (static)
 * ------------------------------
 * Processes the line containing a label definition during the first pass of the assembly code.
 * It validates and handles the label statement, updates the data structures, and performs necessary checks.
 * This function is used after verifying that the 'name_of_label:' statement is valid.
 * The function may update the linked list of defined labels (head) by adding a new label node.
 * The function may print warnings or errors if it encounters issues with the label definition or subsequent statements.
 * ------------------------------
 * Parameters:
 *   1: A pointer to the beginning of the line containing the label.
 *   2: The length of the label statement (excluding the ':' character).
 *   3: A pointer to an integer indicating whether an error occurred during processing.
 *   4: The name of the input assembly file being processed.
 *   5: The line number of the current line in the assembly file.
 *   6: A pointer to the linked list head for defined labels.
 *   7: The address associated with the current line in the assembly file.
 * ------------------------------
 * Returns:
 *   If the label is successfully processed without any errors or conflicts:
 *     - The function returns the number of characters from the beginning of the line till the first character of the opcode/data/string statement.
 *   If an error occurs during processing or the line has no meaningful content after the label definition:
 *     - The function returns ILEGAL (-999), indicating an error, and the error_flag will be updated.
 * ------------------------------
 * static int handle_def_label_line(const char *p, int length, int *error_flag, const char* file_name, int line_number, def_label_ptr *head, int address) ;
 */


#endif /* FIRST_PASS_H */
