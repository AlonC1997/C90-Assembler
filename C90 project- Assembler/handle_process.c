/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * Date of Submission: 10/08/2023
 * -------------------------
 * File name: handle_process.c
 * -------------------------
 * This file is responsible for:
 * - Handling file opening, reading, and closing operations.
 * - Managing the overall flow of the assembler process.
 * - Orchestrating the execution of different stages (macro handling, first pass, second pass).
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/first_pass.h"
#include "headerfiles/general_helpers.h"
#include "headerfiles/error.h"
#include "headerfiles/handle_macro.h"
#include <stdio.h>
#include <string.h>

/* Functions: */
int handle_process(const char * file_name) {
    FILE * open_f ; /* '.as' file*/
    FILE * process_f ; /* '.am' file*/
    FILE * ob_f ; /* '.ob' file*/
    FILE * ext_f ; /* '.ext' file*/
    FILE * ent_f ; /* '.ent' file*/
    char open_f_name[MAX_FILE_NAME] ;
    char process_f_name[MAX_FILE_NAME] ;
    char ob_f_name[MAX_FILE_NAME] ;
    char ext_f_name[MAX_FILE_NAME] ;
    char ent_f_name[MAX_FILE_NAME] ;

    strcpy(open_f_name,file_name) ; /* Making the name of the input('.as') file start without extension. */
    strcpy(process_f_name,file_name) ; /* Making the name of the output ('.am') file start without extension. */
    strncat(open_f_name, AS_EXTENTSION,EXTENSION_LENGTH) ; /* Adding relevent extension for input file('.as'). Project instructions. strncat is autopading with '\0'. */
    strncat(process_f_name, AM_EXTENTSION,EXTENSION_LENGTH) ; /* Adding extension to output and first/second pass analyze file('.am'). Project instructions. strncat is autopading with '\0'. */

    if (!(open_f = fopen(open_f_name, "r"))){
        file_error(FILE_OPEN_ERROR, open_f_name, DEFAULT_VALUE) ; 
        return TRUE ; /* Error. Process will get next file.*/
    }
    if (!(process_f = fopen(process_f_name, "w"))) {
        file_error(FILE_OPEN_ERROR, process_f_name, DEFAULT_VALUE) ; 
        return TRUE ; /* Error. Process will get next file.*/
    }
    if (handle_as_file(open_f_name, open_f, process_f) == FALSE) {
        fclose(open_f) ;   
        fclose(process_f) ; 
        if (remove(process_f_name) != DEFAULT_VALUE) { /* Delete .am file because of normal error('.am' will not be created).  */
            file_error(DELETE_FILE_ERROR, file_name, DEFAULT_VALUE) ; /* Error when trying to delete. Fattal error.*/
            return FALSE ; /* Fattal error. Process wont continue even if we have more files to analyze.*/
        }
        else { /* Not Fattal error but there is an error with the contents of the file '.as' so we cant make the '.am' file... Getting next file input (if there is). */
            return TRUE ; 
        }
    }
    else { /* handles_as_file (macros spreading) worked well and created '.am' file. Starting first_pass process, we dont need '.as' file anymore. */
        fclose(open_f) ; /* Dont need this file anymore working only on '.am' file. */      
    
        strcpy(ob_f_name,file_name) ; /* Making the name of the output ('.ob') file start without extension. */
        strcpy(ext_f_name,file_name) ; /* Making the name of the output ('.ext') file start without extension. */
        strcpy(ent_f_name,file_name) ; /* Making the name of the output ('.ent') file start without extension. */
        strncat(ob_f_name, OB_EXTENTSION,EXTENSION_LENGTH) ; /* Adding extension to output and first/second pass analyze file('.ob'). Project instructions. strncat is autopading with '\0'. */
        strncat(ext_f_name, EXT_EXTENTSION,EXTENSION_LENGTH+1) ; /* Adding extension to output and first/second pass analyze file('.ext'). Project instructions. strncat is autopading with '\0'. */
        strncat(ent_f_name, ENT_EXTENTSION,EXTENSION_LENGTH+1) ; /* Adding extension to output and first/second pass analyze file('.ent'). Project instructions. strncat is autopading with '\0'. */

        if (!(ob_f = fopen(ob_f_name, "w"))){
            file_error(FILE_OPEN_ERROR, ob_f_name, DEFAULT_VALUE) ; 
            return TRUE ; /* Error. Process will get next file. */
        }
        if (!(ext_f = fopen(ext_f_name, "w"))){
            file_error(FILE_OPEN_ERROR, ext_f_name, DEFAULT_VALUE) ; 
            return TRUE ; /* Error. Process will get next file. */
        }
        if (!(ent_f = fopen(ent_f_name, "w"))){
            file_error(FILE_OPEN_ERROR, ent_f_name, DEFAULT_VALUE) ; 
            return TRUE ; /* Error. Process will get next file. */
        }

        /* Reopen process ('.am') file (after macros spreading) with reading only mode parameter. */    
        fclose(process_f);
        process_f = fopen(process_f_name, "r");
        if (process_f == NULL) {
            file_error(FILE_OPEN_ERROR,process_f_name , DEFAULT_VALUE) ;
            return TRUE ;
        }

        /* handle_first_pass will call handle_second_pass if worked well (worked well == else case == return TRUE). */
        if (handle_first_pass(process_f_name, process_f, ob_f, ext_f, ent_f) == FALSE) { /* error indicate at first and second passes processes. No file will create. getting next input file. */
            fclose(process_f) ;
            fclose(ob_f) ;
            fclose(ext_f) ;
            fclose(ent_f) ;
            if (remove(ob_f_name) != DEFAULT_VALUE || remove(ext_f_name) != DEFAULT_VALUE|| remove(ent_f_name) != DEFAULT_VALUE) { /* Delete files because of normal error(all files will remove). */
                file_error(DELETE_FILE_ERROR, file_name, DEFAULT_VALUE) ; /* Error when trying to delete. Fattal error.*/
                return FALSE ; /* Fattal error. Process wont continue even if we have more files to analyze. */
            } 
            else { /* Not Fattal error but there is an error indicate in first pass so second pass will not happen.... Getting next file input (if there is). */
                return TRUE ; 
            }
        }
        else { /* First pass (which call also to second pass worked well) proccess worked well. Zero errors indicate.*/
            fclose(process_f) ;
            fclose(ob_f) ;
            fclose(ext_f) ;
            fclose(ent_f) ;
            return TRUE ;
            /* Whole proccess worked well. Zero errors indicate. All require files were created. Getting next file input if exist.  */    
        }
    }
}
