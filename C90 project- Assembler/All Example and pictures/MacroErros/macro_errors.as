;This file shows all examples of macro_pass_error that can be printed.
;';' Indicates the type of error and the detail of what will be printed.


;case END_MACRO_WITHOUT_DEF:
;Can't close mcro without open one.
  mov @r3, @r4
endmcro


;case NO_DEF_FOR_MACRO: 
;Can't spread mcro without definition.
 m2


;case MACRO_DEF_WITHOUT_NAME:
;Can't define mcro without giving a name.
;Also this lines will print: 
;"Can't close mcro without open one" (Because the definition didnt work).
 mcro   
    bne LOOP
END: stop
  endmcro


;case NAME_OF_MACRO_LIKE_DIRECTIVE: 
;Name of macro cant be like directive.
 mcro .entry
  bne  LOOP


;case NAME_OF_MACRO_LIKE_FUNCTION:
;Name of macro cant be like function.
 mcro  mov
   bne    LOOP


;case NAME_OF_MACRO_LIKE_REGISTER:
;Name of macro cant be like register.
;Also this lines will print: 
;"Can't close mcro without open one" (Because the definition didnt work).
mcro @r1 
 bne LABEL
endmcro


;case MACRO_NAME_LINE_ISNT_EMPTY_AFTER_NAME:
;The macro spreading line should be empty (Only white chars is legal) after ' $Macro-name$ '.
;Also this comment line will print: 
;Line is grater then 80.'.am' file will not be created and also
m1 3


;case DEF_LINE_ISNT_EMPTY_AFTER_NAME:
;The defintion macro command line should be empty (Only white chars is legal) after 'mcro $Macro-name$'.
;Also this comment line will print: 
;Line is grater then 80.'.am' file will not be created and also
;Also this lines will print:
;Can't close mcro without open one Because the definition didnt work.
   mcro     k3          5
;legal comment line inside macro will be delete also.
   bne    END
   endmcro 


;case END_MACRO_LINE_ISNT_EMPTY_AFTER_ENDMCRO:
;The end macro command line should be empty (Only white chars is legal) after ' endmcro '.
;Also this comment line will print: 
;Line is grater then 80.'.am' file will not be created and also
    mcro     k6        
   bne    END
   endmcro 3
   endmcro
 k6  


;case MACRO_CANT_REDEFINED:
;Macro can not redefined.
mcro     m1         
   bne    END
   endmcro          
mcro m1 
 mov @r3, @r1
 end mcro
;Also this line will print:  Can't close mcro without open one.
    endmcro