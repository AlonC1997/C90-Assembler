;This file shows all examples of first pass label errors that can be printed.
;';' Indicates the type of error and the detail of what will be printed.

;Note:
;In case you miss the assumes at the code:
;For the avoidance of doubt:
;Registered in the forum that the label must not 
;be registered under any circumstances. 
;For example: 'r1' is forbidden 
;(of course not '@r1' either because it doesn't start with a letter).
;In addition, it is noted that the label is 
;allowed to be a directive without the dot at the beginning.
;For example 'extern' but not '.extern'.
;Therefore, the option that a label will be called by the name of a 
;does not exist in the test at all because for example:
;'string' is valid for a label name and '.string' is invalid
;because it does not start with a letter.



;case LABEL_NAME_SHOULD_START_WITH_LETTER:
;Label name should start with a letter.
1x: mov @r1, @r2
        

;case LABEL_NAME_CONTAIN_ONLY_LETTERS_AND_NUMBERS:
;Label name should contain only letters and numbers.
x/: mov @r1, @r2


;case LABEL_MAX_LENGTH_IS_31: (The example has 32 chars)
;Label max length is 31.
VeryVeryVeryVeryVeryVeryVeryLong: mov @r1, @r2


;case LABEL_NAME_CANT_BE_AS_OPCODE_NAME:
;Label name can't be an OPCODE name.
mov: mov @r1, @r2


;case LABEL_NAME_CANT_BE_AS_REGISTAR_NAME:
;Label name can't be a register name.
r1: mov @r1, @r2


;case LABEL_NAME_CANT_REDEFINED:
;Label name can't be redefined.
L1: mov @r1, @r2
L1: mov @r1, @r3


;case LABEL_CANT_BE_BOTH_EXTERNAL_AND_ENTRY:
;Label can't be both external and entry.
.extern labeL2
.entry labeL2


;case LABEL_CANT_DEFINED_LABEL:
;Label name can't define a label.
LABEL3: LABEL4: mov @r1, @r2


;case TWO_CONSECUTIVE_COLON:
;Illegal two consecutive colons.
LABEL5:: mov @r1, @r2


;case MISSING_COLON:
;A label name must end with consecutive colons ':'.
LABEL6 : mov @r1, @r2