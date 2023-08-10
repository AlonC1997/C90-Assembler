#ifndef HANDLE_MACRO_H
#define HANDLE_MACRO_H

/* Headears include: */
#include <stdio.h>

/* Functions: */
/*
 * Function: handle_as_file
 * -------------------------
 * Handling ".as" file and and fill in the ".am".
 * -------------------------
 * Assume: line isnt greater than 8O chars (not include '\0' char). confrimed by lecturer Ester at the forum. 
 * Assume: if encounter empty line ---> delete it and getting next line. 
 * Assume: delete each space untill first non white char. confrimed by Roei Rahmany at the forum. 
 * Assume: Illegal comment line (white chars before ;) will be detect at first pass (because maybe there is more errors that i need to detect at first pass),
 * but legal comment line will be delete at macro spreading pass, confrimed by Ester.
 * -------------------------
 * Mamman instructions: no white chars before ';' char at comment line. 
 * -------------------------
 * Parameters:
 *   1: File name (without '.as' extension).
 *   2: Pointer to '.as' file.
 *   3: Pointer to '.am' file to spreading the macros in. 
 * -------------------------
 * Returns:
 *   TRUE(1)- Worked well. ('.am' file will be created).
 *   FALSE(0)- Not Worked well: ('.am' file will not be created).
 * -------------------------
 */
int handle_as_file(const char *, FILE *, FILE *) ;

/*
 * Function: handle_as_file_line (static)
 * ----------------------
 * Checking the content of line from ".as" file and indicate for errors detected. 
 * Responsible for the macros flag.
 * -------------------------
 * Assume: each macro definition has a closure (Mamman instructions). On the other hand if there is endmcro without opening ('mcro $macro-name$') error will be printed. 
 * ----------------------
 * Mamman instructions: there is no errors inside the macro body, but in the define line('mcro $macro-name$') or end macro line ('endmcro') errors can occur. 
 * Mamman instructions: macro can be any name include numbers and signs if its not command\function name\directive name\label name. 
 * Mamman instructions: macro can be defined once. twice (or more) --> error. 
 * Mamman instructions: there are no nested macro definitions (The lecturer Roni Ben Ishai told me  to assume that in the forum). 
 * Mamman instructions: there is not a macro name (spreading call) before its definition. (The lecturer Michal avimor writes that in the forum) 
 * ----------------------
 * Parameters:
 *   1: Pointer to first char of line.
 *   2: File name is always for sent to print the file name in case of error.
 *   3: Line number is always sent to print the line number in case of error.
 *   4: Pointer to first element in connected list of macros in '.as' file.
 *   5: Pointer to last element in connected list of macros in '.as' file.
 *   6: Changeable flag of macro def detecting.
 *   7: Changeable flag of erorr detecting. 
 * -------------------------
 * Return:
 *   COPY_AS_IS(1)- indication to copy this line to '.am' file.
 *   DONT_COPY(2)- indication not to copy the row to '.am' file.
 * -------------------------
 * static int handle_as_file_line(char *, const char *, int, macro_ptr *, macro_ptr *, int *) ; 
 */

/*
 * Function: more_than_one_sequence_case (static)
 * ----------------------
 * Handling case of two+ sequence/two singles charc with space in-between, in line from ".as" file and indicate for errors detected. 
 * -------------------------
 * Parameters:
 *   1: Pointer to first char of line.
 *   2: File name is always for sent to print the file name in case of error.
 *   3: Line number is always sent to print the line number in case of error.
 *   4: Pointer to first element in connected list of macros in '.as' file.
 *   5: Pointer to last element in connected list of macros in '.as' file.
 *   6: Changeable flag of def macro detecting.
 *   7: Changeable flag of erorr detecting. 
 *   8: Number of consecutive non white chars from start (calculated in the sender function, so i didnt want to do this twice- to save runtime.)
 * -------------------------
 * Return:
 *   COPY_AS_IS(1)- indication to copy this line to '.am' file.
 *   DONT_COPY(2)- indication not to copy the row to '.am' file.
 *   MACRO_NAME(3)- indication to macro name which represent spreading line on '.am' file.
 * -------------------------
 * static int more_than_one_sequence_case(char *, const char *, int, macro_ptr * ,macro_ptr * , int *, int) ;
 */

/*
 * Function: one_sequence_case (static)
 * ----------------------
 * Handling case of one sequence/or one chars, in line from ".as" file and indicate for errors detected. 
 * -------------------------
 * Parameters:
 *   1: Pointer to first char of line.
 *   2: File name is always for sent to print the file name in case of error.
 *   3: Line number is always sent to print the line number in case of error.
 *   4: Pointer to first element in connected list of macros in '.as' file.
 *   5: Pointer to last element in connected list of macros in '.as' file.
 *   6: Changeable flag of def macro detecting.
 *   7: Changeable flag of erorr detecting. 
 *   8: Number of consecutive non white chars from start (calculated in the sender function, so i didnt want to do this twice- to save runtime.)
 * -------------------------
 * Return:
 *   COPY_AS_IS(1)- indication to copy this line to '.am' file.
 *   DONT_COPY(2)- indication not to copy the row to '.am' file.
 *   MACRO_NAME(3)- indication to macro name which represent spreading line on '.am' file.
 * -------------------------
 * static int one_sequence_case(char * , const char * , int , macro_ptr *, macro_ptr *, int *, int) ;
 */

/*
 * Function: is_it_end_macro (static)
 * -------------------------
 * Checks if a sequence of characters is a "endmcro" string.
 * -------------------------
 * Parameters:
 *   1: Pointer to first char of string to check.
 *   2: Number of consecutive chars to compare with.
 * -------------------------
 * Returns:
 *   TRUE(1)- if it is "endmcro" string.
 *   FALSE(0)- if it is not "endmcro" string.
 * -------------------------
 * static int is_it_end_macro (const char *, int) ;
*/

/*
 * Function: is_it_macro_def (static)
 * ----------------------
 * Checking if a sequence of characters is a "mcro" which means start of macro definition.
 * -------------------------
 * Parameters:
 *   1: Pointer to first letter to check.
 *   2: Max length to check.
 * -------------------------
 * Returns:
 *   TRUE(1)- it's macro definition.
 *   FALSE(0)- it's not definition.
 * -------------------------
 * static int is_it_macro_def(const char *, int) ;
 */

/*
 * Function: is_it_macro_name (static)
 * ----------------------
 * Checking if a sequence of characters is a macro name.
 * -------------------------
 * Parameters:
 *   1: Pointer to first letter to check.
 *   2: Max length to check.
 *   3: Pointer to first element in macros linked list.
 * -------------------------
 * Returns:
 *   TRUE(1)- it's macro name.
 *   FALSE(0)- it's not name.
 * -------------------------
 * static int is_it_macro_name(const char *, int, macro_ptr *) ;
 */


#endif /* HANDLE_MACRO_H */
