#ifndef HANDLE_PROCESS_H
#define HANDLE_PROCESS_H

/* Functions: */
/*
 * Function: handle_process
 * ----------------------
 * Open new file which we got on param1 with adding ".as" extension and send to:
 * Step1- handle macro spreading --> Step2- first_pass (if step1 worked well- .'am' file created. ) --> Step3- Second_Pass (if step2 worked well) --> 
 * Output_Files (".ob" , ".ext" , ".ent" files) --> main (next ".as" file to analyze if exist) --> Routine until no more argument in main.
 * ---------------------- 
 * Parameters:
 *   1: The file name (without open extension).
 * ----------------------
 * Returns:
 *   TRUE(1)- if everything is worked well.
 *   FALSE(0)- if not worked well. Main will terminate.
 * -------------------------
 */
int handle_process(const char *) ;


#endif /* HANDLE_PROCESS_H */
