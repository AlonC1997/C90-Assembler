;This file shows all examples of second pass errors can be printed.
;';' Indicates the type of error and the detail of what will be printed.


; In this transition there are two possible errors:
;Please note that only one of them will be possible 
;due to the initial check process of the ent file layout, 
;if the first one is found we will not get to the test
;phase of the second one at all.
;If you would like to see the second error, 
;please delete the first error from this file.


;FIRST:
;case ENTRY_LABEL_WASNT_DEFINED_AT_FILE:
;Entry label wasn't defined at file.
.entry Label2


;SECOND: (Delete first ('.entry Label2' line) if you want to see this error).
;case NO_SUCH_LABEL_DEFINED:
;No label defined.
mov @r1, label