;This file shows all examples of first pass general errors that can be printed.
;';' Indicates the type of error and the detail of what will be printed.


;case COMMENT_LINE_ILLEGAL_SPACES:
;Comment line char ' ; ' should be the first char in the line 
;without white chars before at all.
  ;

;case ADDITIONAL_UNRELATED_TEXT:
;Unrelated characters at the end of the text.
mov @r3, @r4  d


;case TWO_CONSECUTIVE_COMMAS:
;Illegal two consecutive commas
mov @r3,, @r4


;case ILEGAL_COMMA: 
;Illegal place for a comma. 
mov ,@r3, @r4


;case MISSING_COMMA:
;Missing comma after a parameter.
mov @r3 @r4


;case MISSING_SOURCE_PARAMTER:
;Missing source parameter.
lea @r1


;case MISSING_TARGET_PARAMTER:
;Missing target parameter.
mov @r3,


;case A_NUMBER_SHOULD_APPEAR_AFTER_PLUS_OR_MINUS:
;A number must appear after '+' or '-'.
mov +, @r4


;case UNAPROPIATE_SOURCE_PARAMETER:
;Unappropriate source parameter.
lea +3, @r1


;case UNAPROPIATE_TARGET_PARAMETER:
;Unappropriate target parameter.
mov @r3, -4


;case ONLY_INTEGERS_NUMBERS_ARE_ACCEPT:
;Only integer numbers are acceptable
mov 4.2, @r1


;case NAME_OF_REGISTAR_IS_MISSING_AFTER_ATSIGN:
;Missing register name after '@'.
mov @ ,@r1

        
;case ILEGAL_REGISTER_NAME: (X3)
;Register name should be only one of those: 'r1' 'r2' 'r3' 'r4' 'r5' 'r6' 'r7'
mov @10 ,@r1
mov @r1 ,@r124
mov @f , @r2