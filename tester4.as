; This is a long tester assembly file
; It includes different types of instructions, labels, and macros

; External and Entry Declarations
.extern EXT1, EXT2
.entry START, LOOP

; Data Declarations
.data 3, -5, 12, 15, -8
.string "Hello, World!"
.data 0, -32768, 32767 ; Edge cases for 15-bit signed integers

; Start of the program
START:  mov r1, r2
        add r3, #10
        sub r4, r5
        cmp r6, EXT1
        lea STR, r7
        clr r0

; Macro definitions
macr MACRO1
        mov r1, r2
        inc r2
        add r1, r3
        dec r4
endmacr

macr MACRO2
        dec r5
        cmp r2, r3
        clr r1
endmacr

macr MACRO3
        jmp END
        not r3
endmacr

; Using macros
MAIN:   MACRO1
        MACRO2

; More instructions
        prn #48
        jsr SUBROUTINE
        red r6
        mov *r6, EXT2
        sub r1, r4
        cmp r3, #-6
        bne END
        add r7, *r6
        clr K
        sub EXT2, EXT2
        jmp LOOP

; Loop and subroutine
LOOP:   prn r5
        MACRO3
        stop

SUBROUTINE:
        lea STR, r5
        jsr PROCESS
        rts

PROCESS:
        mov r1, r2
        add r3, r4
        rts

; End of the program
END:    stop

; String and data
STR:    .string "Example string"
LIST:   .data 6, -9, 100, -32768, 32767

K:      .data 31
.extern L3
.entry LIST

