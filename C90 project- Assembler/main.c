/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * Date of Submission: 10/08/2023
 * -------------------------
 * File name: main.c
 * -------------------------
 * This file is responsible for:
 * - Controlling the flow of the assembler program.
 * - Handling command-line arguments and file names.
 * - Invoking different stages of the assembler process.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/general_helpers.h"
#include "headerfiles/handle_process.h"
#include <stdlib.h>

/* 
 * Function: main
 * ----------------------
 * Sending files name to do the process- file after file.
 * ----------------------
 * Encapsulation principles:
 *  - All information is hidden from the user.
 *  - Throughout the project the principles of encapsulation were implemented. 
 *  - The user does not have direct access to the data.
 * ----------------------
 * To organize the C files:
 * General function documentation will be done in the Hadar file, of course internal documentation will always appear (for both static and non static function).
 * ----------------------
 * Assume (which relevant for all the procees):
 * If there is a file memmory-allocation/Delete file/ferror errors (which are fattal for this project) the process ends immediately (an appropriate error will be printed). 
 * Any other errors will be printed and the following input file will be parsed.
 * ----------------------
 * If there are assumptions that are not listed in the project requirements, they will appear in the corresponding functionality documentation.
 * If there are instructions from the course team that appeared in the forum, they will appear in the file documentation
 * ----------------------
 * Course team forum instruction: global variables are not allowed in all the project.
 */
int main(int argc, char * argv[]) {
    int i ;
    int to_do ;
    to_do = TRUE ;
    i = DEFAULT_VALUE ; 
    if (argc < MIN_ARGC) {
        exit(FALSE) ; /* exit(0) indicate a successful termination of the program.
        I chose this type because it's not that something bad happened, it's a problem with the user's input. */
    } 
    for (i = 1 ; i < argc ; i++ ) {
        to_do = handle_process(argv[i]) ;
        if (to_do == FALSE) {
            exit(TRUE) ; /* exit(1) used to indicate an error or abnormal termination of the program like file removing/no EOF finding/error indicator become set(ferror) errors. */
        }
        else {
            continue ;
        }
    } 
    return DEFAULT_VALUE ;
}
