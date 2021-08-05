; Example from book, Figure 7.1, page 179
; Program to multiply an integer by the constant 6.
; Before execution, an integer must be stored in
; NUMBER.
        .ORIG x3050
        LD R1, SIX
        LD R2, NUMBER
        AND R3, R3, #0 ;; clear R3; will contain product

;; Loop

AGAIN   ADD R3, R3, R2
        ADD R1, R1, #-1
        BRp AGAIN

        HALT

NUMBER  .BLKW 1
SIX     .FILL x0006

        .END
