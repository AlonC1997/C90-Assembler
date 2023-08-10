/*
 * Mamman number: 14 (Project - Assembler in c90).
 * Name: Alon Carmel.
 * ID: 319004305.
 * Course number: 20465.
 * Date of Submission: 10/08/2023
 * -------------------------
 * File name: second_pass.c
 * -------------------------
 * This file is responsible for:
 * - Implementing the second pass of the assembler process.
 * - Handling the resolution of addresses, symbols, and operands.
 * -------------------------
 */

/* Headears include: */
#include "headerfiles/error.h"
#include "headerfiles/general_helpers.h"
#include "headerfiles/output_files.h"
#include <string.h>

/* Functions: */
static int check_for_not_def_entry_label(def_label_ptr *h, const char *file_name, FILE *ent_f) {
    def_label_ptr temp ;
    int error_flag ;
    int first_flag ; /* Flag to indicate if its first line of this file - helping us avoid extra white line in the end of this file. */
    first_flag = TRUE ;
    error_flag = FALSE ;
    temp = *h ;

    while (temp != NULL) {
        if (def_label_node_get_entry_flag(temp) && !(def_label_node_get_regular_flag(temp))) {
            second_pass_error(ENTRY_LABEL_WASNT_DEFINED_AT_FILE, file_name, def_label_node_get_def_line(temp)) ;
            error_flag = TRUE; /* We want to print maximum numbers of errors, so we checking all list items and not return from function till done. */
        }
        if (def_label_node_get_entry_flag(temp) == TRUE) {
            print_label_on_ent(ent_f, def_label_node_get_name(temp), def_label_node_get_address(temp), first_flag) ;
            first_flag =  FALSE ;
        }
        temp = def_label_node_get_next(temp) ;
    }
    if (error_flag == TRUE) {
        return FALSE ;    
    }
    else { /* No errors encountered, all entry labels were defined at file. */
        return TRUE ;    
    }
}

int handle_second_pass(FILE *ob_f, FILE *ext_f, FILE *ent_f, main_ptr *main_head, data_image_ptr *data_head, def_label_ptr *def_head, int ic, int dc, const char *file_name) {
    def_label_ptr def_temp = NULL ; 
    main_ptr main_temp = NULL ; 
    data_image_ptr data_temp = NULL ;
    int first_flag ; /* Flag to indicate if its first line of this file - helping us avoid extra white line in the end of this file. */
    int error_flag ;
    first_flag = TRUE ;   
    error_flag = FALSE ;
    if (*def_head != NULL) {
        def_temp = *def_head;
    }
    if (*main_head != NULL) {
        main_temp = *main_head;
    }
    if (*data_head != NULL) {
        data_temp = *data_head;
    }
    /* Starting second pass process. */
    if (!check_for_not_def_entry_label(&def_temp, file_name, ent_f)) { /* Case 1: Found used label which never defined. */
        return FALSE ; /* Error was printed. No file will be created. */
    }
    else { /* Case 2: No errors encountered at all process, '.ent' file created. '.ob' and '.ext' files will be created if all labels that used as parameters are defined. */
        print_ob_title(ob_f, ic, dc) ;
        while (main_temp != NULL) {
            if (main_node_get_type(main_temp) == STRING || main_node_get_type(main_temp) == DATA) { /* Case 2.1: '.string' or '.data' line. */
                print_data(ob_f, &data_temp, main_node_get_start_address(main_temp)) ;
                main_temp = main_node_get_next(main_temp) ;
                continue ;
            }
            else { /* Case 2.2: Not data or string line. */
                first12_bits_to_base64(main_node_get_first_line_code(main_temp), ob_f) ; /* All types except for extern/entry/string/data have this line code. */
                
                /* Source operand handling. */
                if (main_node_get_source(main_temp) == NONE_ADDRESS && main_node_get_target(main_temp) == NONE_ADDRESS) { /* Case 2.2.1: rts/stop opcodes, one code line. Already printed. */
                    main_temp = main_node_get_next(main_temp) ;
                    continue ;
                }
                else if (main_node_get_source(main_temp) == NONE_ADDRESS && main_node_get_target(main_temp) == IMMEDIATELY_ADDRESS) { /* Case 2.2.2: prn. */
                    first12_bits_to_base64(main_node_get_second_line_code(main_temp), ob_f) ;
                    main_temp = main_node_get_next(main_temp) ;
                    continue ;
                }
                else if (main_node_get_source(main_temp) == NONE_ADDRESS && main_node_get_target(main_temp) == DIRECT_REGISTER_ADDRESS) { /* Case 2.2.3: prn/not/clr/inc/dec/jmp/bne/red/jsr opcodes. */
                    first12_bits_to_base64(main_node_get_second_line_code(main_temp), ob_f) ;
                    main_temp = main_node_get_next(main_temp) ;
                    continue ;
                }
                else if (main_node_get_source(main_temp) == NONE_ADDRESS && main_node_get_target(main_temp) == DIRECT_ADDRESS) { /* Case 2.2.4: prn/not/clr/inc/dec/jmp/bne/red/jsr opcodes. */
                    if (!check_and_print_labels(ob_f, ext_f, (main_node_get_start_address(main_temp)) + 1, &def_temp, main_node_get_target_name(main_temp), strlen(main_node_get_target_name(main_temp)), file_name, main_node_get_line_number(main_temp), &first_flag)) {
                        error_flag = TRUE;  /* Errors found and printed. */
                    }        
                    /* Label is at start address cell +1. */
                    main_temp = main_node_get_next(main_temp) ;
                    continue;
                }

                else if (main_node_get_source(main_temp) == IMMEDIATELY_ADDRESS) { /* Case 2.2.5: Num. */
                    first12_bits_to_base64(main_node_get_second_line_code(main_temp), ob_f) ;
                }
                else if (main_node_get_source(main_temp) == DIRECT_REGISTER_ADDRESS) { /* Case 2.2.6: Register. */
                    first12_bits_to_base64(main_node_get_second_line_code(main_temp), ob_f) ;
                    if (main_node_get_target(main_temp) == DIRECT_REGISTER_ADDRESS) {
                        main_temp = main_node_get_next(main_temp) ;
                        continue ; /* Two register operands are coded in one line (second_line_code). */
                    }
                }
                else if (main_node_get_source(main_temp) == DIRECT_ADDRESS) { /* Case 2.2.7: Source is label, could be mov/cmp/add/sub/lea opcodes. */
                    if (!check_and_print_labels(ob_f, ext_f, (main_node_get_start_address(main_temp)) + 1, &def_temp, main_node_get_source_name(main_temp), strlen(main_node_get_source_name(main_temp)), file_name, main_node_get_line_number(main_temp), &first_flag)) {
                        error_flag = TRUE ; /* Errors found and printed. */
                    }
                    /* Label is at start address cell +1. Checking target operand now. */
                }
                /* Target operand handling. */
                if (main_node_get_target(main_temp) == IMMEDIATELY_ADDRESS) { /* Case 1 (Target opernads): cmp opcode, target is number or label. Other option for num in target already checked. */
                    first12_bits_to_base64(main_node_get_third_line_code(main_temp), ob_f) ;
                    main_temp = main_node_get_next(main_temp) ;
                    continue ;
                }
                else if (main_node_get_target(main_temp) == DIRECT_REGISTER_ADDRESS) { /* Case 2 (Target opernads): Target is register, could be mov/cmp/add/sub/lea opcodes. */
                    first12_bits_to_base64(main_node_get_third_line_code(main_temp), ob_f) ;
                    main_temp = main_node_get_next(main_temp) ;
                    continue ;
                }
                else if (main_node_get_target(main_temp) == DIRECT_ADDRESS) { /* Case 3 (Target opernads):Target is label, could be mov/cmp/add/sub/lea opcodes. */
                    if (!check_and_print_labels(ob_f, ext_f, (main_node_get_start_address(main_temp)) + 2, &def_temp, main_node_get_target_name(main_temp), strlen(main_node_get_target_name(main_temp)), file_name, main_node_get_line_number(main_temp), &first_flag)) { /* Label is at start address cell +2. */
                        error_flag = TRUE ; /* Errors found and printed. */
                    }
                    main_temp = main_node_get_next(main_temp) ; 
                    continue ;
                }
            }
        }
    } /* Done spreading files. Now checking for flag_errors. */
    if (error_flag == TRUE) { /* Case 3: Errors found and printed. Only '.am' will be created. */
        return FALSE ;
    }
    else { /* Case 4: No errors at all, all files will be created. */
        return TRUE ;
    }
}
