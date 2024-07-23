; This is a comment line
START:    mov r3, r5
          add r2, r4

          sub r4, r3

; Data Section
.data 1, 3, -4, 16383, -16384

; Illegal Data Section (Out of Range)
.data 1, 3, -4, 16384, -16385 

; String Section
xyz: .string "ascii string"

; Illegal String Section (Missing Quote)
.string "ascii string

; Entry Declarations
.entry LABEL, LABEL2

; Illegal Entry Declaration (Invalid Label)
.entry 1LABEL

; External Declarations
.extern LABEL3, LABEL4

; Illegal External Declaration (Invalid Label)
.extern LABEL,3

END:      stop
