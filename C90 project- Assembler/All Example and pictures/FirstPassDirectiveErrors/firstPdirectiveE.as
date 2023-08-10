;This file shows all examples of first pass directive errors can be printed.
;';' Indicates the type of error and the detail of what will be printed.


;case TWO_CONSECUTIVE_DOT:
;Illegal two consecutive dots.
..extern label
  

;case EXTERNAL_LABEL_CANT_BE_REDEFINED_IN_A_FILE:
;External label can't be redefined in a file.
.extern label1
label1: mov @r1, @r2


;case MISSING_LABEL_AFTER_COMMA_OR_ILEGAL_COMMA_AT_END:
;Missing label after a comma or delete comma at the end.
.entry Label2,


;case EXTERN_IS_EMPTY:
;At least one label name should appear after '.extern'.
.extern


;case ENTRY_CONTAIN_ONE_LABEL_FOLLOWED_BY_NOTHING:
;Entry ('.entry') statement should contain only one label followed by nothing.
.entry label3 ,label4


;case ENTRY_IS_EMPTY:
;Label name should appear after '.entry'.
.entry


;case DATA_LINE_OPERANDS_ARE_INTEGER:
;Data line operands should be integer numbers and commas between them.
.data @r1 , 3


;case MISSING_NUM_AFTER_COMMA_OR_ILEGAL_COMMA_AT_END:
;A number must appear after a comma in the data line or delete comma at the end
.data +2,-2, 


;case STRING_SHOULD_START_WITH_PAIRS_OF_QUOTATION_MARKS:
;String should start with pairs of quotation marks.
.string xyz


;case STRING_SHOULD_END_WITH_PAIRS_OF_QUOTATION_MARKS:
;String should end with pairs of quotation marks.
.string "xyz


;case STRING_DIRECTIVE_CONTAIN_ONE_STRING_FOLLOWED_BY_NOTHING:
;String directive line contains one string followed by nothing.
.string "first" , "second_not_allowed"