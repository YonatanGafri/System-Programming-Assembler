; file tester_long_valid.as

; Entry points and external labels
.entry MAIN
.entry LOOP
.entry STR
.entry DATA
.entry STR2
.entry MORE
.extern L1, L2, L3, L4, EXT1, EXT2

; Main program starts here
MAIN:       mov r3, LIST
            cmp r2, r3
            add r3, LIST
            sub r1, r4
            lea STR, r6
            clr r2
            not r5
            inc r6
            dec r1
            jmp LOOP
            bne END
            red r7
            prn #48
            jsr FN1
            rts
            stop

; Function definitions
FN1:        dec r5
            lea STR, r2
            jmp L2
            rts

; Another loop
LOOP:       prn #123
            cmp r4, r3
            bne FN1
            clr r2
            mov *r1, r5
            jsr FN2

; Function definitions
FN2:        add *r5, r5
            cmp r5, r1
            bne LOOP
            rts

END:        stop

; Data sections
STR:  .string "example string"
LIST: .data 6, -9, 15, 1024, -2048
      .data -100, 42, 84
K:    .data 31

; Additional data sections
STR2: .string "another string"
DATA: .data 1, 2, 3, 4, 5
MORE: .data -16384, 16383
NUMS: .data 12345, -12345
