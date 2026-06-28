; Invalid command
START:   movv r3, r5

; Invalid label declaration (starts with a number)
123LABEL: add r2, r3

; Immediate value out of range (15-bit signed)
         sub r2, #32768

; Missing comma between operands
         add r2 r4

; Extra comma in operand list
         cmp r1,, r5

; Label redefined
LOOP:    lea STR, r6
LOOP:    clr r1

; Invalid addressing mode
         lea #5, r6

; Missing operand
         sub r1,

; Illegal comma
         prn, #48

; Extra text at end of line
         inc r2 extra_text

; Undefined label used
         jmp UNDEFINED_LABEL

; Label exceeds maximum length
VERY_LONG_LABEL_NAME_EXCEEDING_LIMIT: clr r1

; Invalid string definition (missing opening quote)
STR1:    .string abcd"

; Invalid string definition (missing closing quote)
STR2:    .string "efgh

; Too many characters in a line
         lea STR, r6 

; Illegal character in immediate value
         add r1, #5a

; Label declared multiple times
DUPLICATE: clr r2
DUPLICATE: inc r2

; Invalid data definition
DATA1:   .data 5, , 6

; Data value out of range
DATA2:   .data 32768

; Invalid extern declaration (no label specified)
.extern

; Invalid entry declaration (no label specified)
.entry

; Extern label used without definition
         add r3, EXTERNAL_LABEL

; Entry label not declared in file
.entry UNDECLARED_LABEL

; Immediate value out of range (15-bit signed)
         add r2, #-32769

; Invalid character in label
MAIN@:   mov r3, r5

; Invalid addressing mode
         prn *r5

; Entry label not a valid label
.entry 123LABEL

; Invalid operand for clr command
         clr #5

; Missing operand for inc command
         inc 

; Missing operand for dec command
         dec 

; Invalid operand for not command
         not #5

; Invalid operand for rts command
         rts r3

; Invalid operand for stop command
         stop r3

; Missing operand for jmp command
         jmp 

; Invalid operand for bne command
         bne #5

; Invalid operand for jsr command
         jsr #5

; Invalid register addressing mode
         mov r8, r3

; Invalid register addressing mode
         add r2, r9
