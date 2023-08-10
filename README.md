# C90-Assembler
End of course project:

C project 12 bit Assembler (C90 standard).

The open university of Israel- final project for course number 20465- System Programming Laboratory.

Assembler for C90 language, which includes over 70 different types of errors and warnings.

All information in the project is hidden from the user and the principles of encapsulation have been fully implemented.

The assembler includes over 70 different types of errors and warnings (printed in the file error.c file and you can see the details and examples in the folder: output__files_for_example).

Program inputs:

-File names to parse without the (.as) extension, the program will add the extension independently.

Program outputs:

-The main output is presented in the (.ob) object file you get after converting 12 bits to base64 (this file will be created if and only if no errors occurred at all).

-In addition, there are 3 different output files that will accompany the program, the stages of which are as follows:

-If no errors were detected in the macro layout transition--> .am.

-'.am' is the file after spreading macros, deleting white line, delete spaces until the first non-white character appears and deleting comment lines.

Note: correct command line is only if - ';' (which marks a comment line) will appear as the first character without spaces before, otherwise the comment line is incorrect and this error will be detected during the first pass as required.

-If the the macros spreading was carried out correctly and the '.am' file was created ---> first pass will start. (The .am file will be the file to be analyzed in the first pass).

-If no errors were detected in the first or second pass --> '.ob' '.ext' '.ent' files will be created.

-In any case, the program has no limitation of input files and every time one file is finished analyzing, it will immediately switch to another one that is received as arguments.

-*If a fatal error occurred- the program will end completely*

Prohibitions: It is forbidden to use stdbool (or any other non c90 standard libary) and therefore I made TRUE & FALSE macros.

I writed the project in the Visul Studio IDE.

In addition, in order to compile the program according to the C90 standard, I used the WSL2 plugin, which behaves exactly like the virtual machine which we were required to implement the program: 

Ubuntu 16.04. No other version is allowed.

valgrind showed 0 leaks and errors on dozens of different runs,  images for example are attached in the output__files_for_example.

Project assumptions are listed throughout.

Full and detailed documentation is accessible throughout the project.

Running images and examples appear in the appropriate folder (output__files_for_example) attached are output files for all types of errors and warnings and also a proper and working main running file.
