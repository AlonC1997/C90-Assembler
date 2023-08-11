*# C90-Assembler*
*Final project for course number 20465- System Programming Laboratory: C project 12 bit Assembler (C90 standard).*

All information in the project is hidden from the user and the principles of encapsulation have been fully implemented.

The assembler includes over 70 different types of errors and warnings (printed in error.c file and you can see the details and examples in the folder: output_files_for_example).

*Program inputs:*

-File names to parse without the ('.as') extension, the program will add the extension independently.

*Program outputs:*

-The main output is presented in the ('.ob') object file you get after converting 12 bits to Base64 (this file will be created if and only if no errors occurred at all).

-In addition, there are 3 different output files that will be created, which are as follows:

-If no errors were detected in the macro spreading pass--> '.am' file will be created.

-'.am' is the file after spreading macros, deleting white lines, deleting leading white characters until the first non-white character appears and deleting --legal comment lines--.

Note: legal comment line is only if ';' (which marks a comment line) will appear as the first character without spaces before, otherwise the comment line is Ilegal and this error will be detected during the first pass as required.

-If the macros spreading was carried out correctly and the '.am' file was created ---> first pass will start. ('.am' file will be the file to be analyzed in the first pass).

-If no errors were detected in the first or second pass --> '.ob', '.ext', '.ent' files will be created.

-In any case, the program has no limitation of input files and every time one file is finished analyzing, it will immediately switch to another one that is received as arguments.

-*If a fatal error occurred- the whole program will end completely and error details will be printed.*

-*Prohibitions: It is forbidden to use stdbool.h (or any other non c90 standard libary) and therefore I made TRUE & FALSE macros.*

I writed the project in the Visual Studio IDE.

In order to compile the program according to the C90 standard, I used WSL2 plugin, which behaves exactly like the virtual machine which we were required to implement the program: Ubuntu 16.04. (No other version is allowed).

Project assumptions are listed throughout.

Full and detailed documentation is accessible throughout the project.

Valgrind showed 0 leaks and errors on dozens of different runs


*Folder output_files_for_example is contain:*

1) Running images and explanation of all errors and warnings.

2) An error-free file, and all its relevant output files.

3) Valgrind running images.
