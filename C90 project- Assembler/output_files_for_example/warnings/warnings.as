;This file shows all examples of warnings that can be printed.
;In this file there are no errors during the transition of the macro spreading.
;Therefore an am file will be created.
;';' Indicates the type of warning and the detail of what will be printed.


;case LABEL_BEFORE_ENTRY:
;No label should appear before '.entry' directive sentence. 
;LABEL10 definition is to avoid printing undefined entry label error.
LABEL1: .entry LABEL10
LABEL10: mov @r1, @r2


;case LABEL_BEFORE_EXTERN:
;No label should appear before '.extern' directive sentence.
LABEL2: .extern LABEL11


;case STRING_IS_EMPTY:
;'.string' directive sentence is empty.
.string 


;case DATA_IS_EMPTY:
;'.data' directive sentence is empty.
.data


;case LABEL_DEFINED_NOTHING:
;The label defined nothing- No content at all, the label will not be saved.  
LABEL3:


;case EXTERN_STATEMENT_TWICE:
;Declaration of extern label twice is unnecessary.   
.extern LABEL4
.extern LABEL4


;case ENTRY_STATEMENT_TWICE:
;Declaration of entry label twice is unnecessary.    
LABEL5: mov @r1, @r2
.entry LABEL5
.entry LABEL5
