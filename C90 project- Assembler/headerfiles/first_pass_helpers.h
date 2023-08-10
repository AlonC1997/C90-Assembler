#ifndef FIRST_PASS_HELPERS_H
#define FIRST_PASS_HELPERS_H

/* Enumerations: */
/* Represents the different addressing methods for operands in assembly instructions. */
typedef enum {
    NONE_ADDRESS = 0, /* Indicates a line with only a target operand or no operands at all. */
    IMMEDIATELY_ADDRESS = 1, /* Immediate addressing method. */
    DIRECT_ADDRESS = 3 , /* Direct addressing method. */
    DIRECT_REGISTER_ADDRESS = 5 /* Direct register addressing method. */
} Address_method ;

/* Represents the different addressing types for symbols in assembly. */
typedef enum {
    ABSOLUTE, /* Symbol has an absolute address. */
    EXTERNAL, /* Symbol is external, its address will be resolved during linking. */
    RELOCATABLE /* Symbol has a relocatable address. */
} ARE;

/* Represents the types of operands used in assembly instructions. */
typedef enum {
    DATA_NUM, /* Operand represents a data number (12-bit range: -2048 to 2047). */
    REGULAR_OPERAND, /* Operand represents a regular operand (10-bit range: -512 to 511). */
    LABEL_OPERAND, /* Operand represents a label operand. */
    REGISTER_OPERAND, /* Operand represents a register operand. */
    ELSE  /* An alternative operand type. */
} Operand_type ;

/* Represents different character indications for parsing purposes. */
typedef enum {
    NUM, /* 0-9*/
    LETTER, /* Capital or small. */
    MATH_SIGN, /* Only '+'or '-' */
    ATSIGN, /* '@' */
    COMMA, /* ',' */
    NULL_CHAR, /* '\0' */
    NONE /* Something else. */
} CHARS_INDICATE ;

/* Represents the different line indications in an assembly program. */
typedef enum {
    MOV, /* "mov" */
    CMP, /* "cmp" */
    ADD, /* "add" */
    SUB, /* "sub" */
    NOT, /* "not" */
    CLR, /* "clr" */
    LEA, /* "lea" */
    INC, /* "inc" */
    DEC, /* "dec" */
    JMP, /* "jmp" */
    BNE, /* "bne" */
    RED, /* "red" */
    PRN, /* "prn" */
    JSR, /* "jsr" */
    RTS, /* "rts" */
    STOP, /* "stop" */
    ENTRY, /* ".entry" */
    EXTERN, /* ".extern" */
    STRING, /* ".string"*/
    DATA, /* ".data" */
    LABEL /* "label indication- saw a legal label name and colon in the end." */
} LINE_INDICATION ;

/* Function: */
/*
 * Function: is_contain_alpha_and_digit_only (static, used only for is_it_legal_label function).
 * -------------------------
 * Checks if a sequence of characters is contain only letters and digits.
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 *   2: Number of consecutive chars to compare with.
 * -------------------------
 * Returns:
 *   TRUE(1)- Contain only letters and digits.
 *   FALSE(0)- Not contain only letters and digits.
 * staic int is_contain_alpha_and_digit_only(const char *, int) ;
*/

/*
 * Function: is_it_legal_number
 * ----------------------
 * This function is doing what atoi does with some chages:
 * Its also print the relevant error.
 * Its refers to the length of the strign to check.
 * Its considers zero as legal on the contrary of atoi function (zero is ilegal sign at atoi).
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to check.
 *   2: length of string to check. 
 *   3: File name, in case of error will be printed.
 *   4: Line number, in case of error will be printed.
 *   5: Error flag to change in case of error.
 *   6: Indicate whether is a .data (12 bits) or regular operand (10 bits) range.
 * -------------------------
 * Returns:
 *   If ilegal: ILEGAL (-999) - Sign to ilegal because our range is -511 to 512 and we need 1 and 0.
 *   If legal: the numberas integer. (like coustom atoi)...
 */
int is_it_legal_number(const char* , int , const char *, int , int *, Operand_type) ;

/*
 * Function: is_it_legal_label (static)
 * ----------------------
 * Checking if a sequence of characters is a legal label name.
 * -------------------------
 * Parameters:
 *   1: Pointer to first letter to check.
 *   2: Consecutive chars to check.
 *   3: File name, in case of error will be printed.
 *   4: Line number, in case of error will be printed.
 *   5: error_flags to update if needed.
 * -------------------------
 * Returns:
 *   TRUE(1)- it's legal label.
 *   FALSE(0)- it's not legal label.
 * -------------------------
 */
 int is_it_legal_label(const char*, int , const char * ,int, int *) ;

/*
 * Function: count_until_next_operand
 * -------------------------
 * This function counts the number of characters in a string until the next operand (non-white and non-comma character).
 * It also checks for errors related to the number of commas encountered in the string.
 * -------------------------
 * Parameters:
 *   1: Pointer to the first character of the string to be analyzed.
 *   2: error_flag: A pointer to an integer flag that will be set to TRUE in case of an error.
 *   3: file_name: The name of the file in which the error occurs (used for error reporting).
 *   4: line_number: The line number in the file where the error occurs (used for error reporting).
 *   5: Type of checking to print exact errors.
 * -------------------------
 * Returns:
 *   If an illegal condition is encountered (-999 ILEGAL):
 *     - If there are two consecutive commas in the string, the function sets the error_flag to TRUE, prints an error message indicating the filename and line number, and returns ILEGAL (-999).
 *     - If there is a missing comma before a non-white, non-comma character, the function sets the error_flag to TRUE, prints an error message indicating the filename and line number, and returns ILEGAL (-999).
 *     - If the function reaches the end of the string after encountering only one non-white character (no second operand), it prints an error message indicating the filename and line number, and returns ILEGAL (-999).
 *   If a legal condition is encountered:
 *     - The function returns the count of characters encountered until the next operand (non-white and non-comma character).
 * -------------------------
 */
int count_until_next_operand(const char*, int*, const char*, int, Operand_type);

/*
 * Function: check_next_non_white_char
 * ------------------------------
 * This function takes a pointer to a string and checks the next non-white character in the string.
 * It then returns the appropriate enum value from the CHARS_INDICATE enumeration based on the type of character found.
 * The function considers characters in the range [0-9] as NUM, alphabetic characters (capital or small) as LETTER, '+' or '-' as MATH_SIGN,
 * '@' as ATSIGN, ',' as COMMA and any other character as NONE.
 * If the string is empty or contains only white spaces, it returns NULL_CHAR.
 * -------------------------
 * Parameters:
 *   1: Pointer to first char to analyze.
 * -------------------------
 * Returns:
 *   The appropriate CHARS_INDICATE enum value indicating the type of the next non-white character in the string.
 *   If the string is empty or contains only white spaces, it returns NULL_CHAR (enum).
 * -------------------------
 */
CHARS_INDICATE check_next_non_white_char(const char* p);

/*
 * Function: count_consecutive_non_white_or_until_first_comma
 * -------------------------
 * This function takes a pointer start of string and counts consecutive non-white characters
 * until it encounters a white character (space, tab, etc.) or the first comma (',') in the string.
 * It stops counting at the first occurrence of a white character or a comma.
 * The function returns the count of consecutive non-white or comma characters encountered.
 * -------------------------
 * Parameters:
 *    1: Pointer to a first char in string to analyze.
 * -------------------------
 * Returns:
 *   The count of consecutive non-white characters encountered until the first white character or comma is found.
 *   The function returns 0 if the first character in the string is a comma (',') or a white character.
 * -------------------------
 */
int count_consecutive_non_white_or_until_first_comma(const char* p);

/*
 * Function: LINE_INDICATION line_indication(const char *p, int consecutive_chars, int *error_flag, const char* file_name, int line_number)
 * -------------------------
 * Determines the type of input string by comparing it with the possible names in the enum and checks if it represents a valid label.
 * The function first checks if the input string is an illegal comment line (starting with spaces).
 * It then compares the input string with the names in the enum to identify the corresponding LINE_INDICATION value.
 * If the input string is exactly equal to one of the possible names, the corresponding LINE_INDICATION value is returned.
 * If the input string is a valid label (ending with ':'), the function checks if it is exactly equal to one of the possible names 
 * and prints an error if it matches. Otherwise, it returns the LABEL (enum) value.
 * If the input string does not match any possible names or is not a valid label, it prints an error message and sets the error_flag to TRUE.
 * -------------------------
 * Parameters:
 *   1: Pointer to the first char of the string to be analyzed.
 *   2: The number of consecutive characters to consider for analysis.
 *   3: A pointer to an integer flag that will be updated to 1 if an error is found, otherwise remains unchanged.
 *   4: The name of the file.
 *   5: The line number in the file.
 * -------------------------
 * Returns:
 *   LINE_INDICATION: The LINE_INDICATION value corresponding to the input string, or ILEGAL (-999, indication to error) if an error is found.
 * -------------------------
 */
LINE_INDICATION line_indication(const char *, int,  int *, const char*, int) ;


#endif /* FIRST_PASS_HELPERS_H */