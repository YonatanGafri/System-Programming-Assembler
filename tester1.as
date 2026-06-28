; This is a test file for assembler
START:    mov r3, r5
          add r2, r4

; Legal Macro Definitions
macr MACRO1
        add #-1 , r1
          mov r1, r2
          inc r2
          add r1, r3
endmacr

macr MACRO2
          dec r5
          cmp r2, r3
endmacr

macr MACRO3
          jmp END
          clr r3
endmacr

; Legal Macro Invocations
MACRO1
MACRO2

          not r5

MACRO3

          sub r4, r3

.data 1, 3, -4, 16383, -16384 

.string "ascii string"

.entry LABEL1, LABEL2

.extern LABEL3, LABEL4

END:      stop
