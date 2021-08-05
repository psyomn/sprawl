    ;; a simple, typical, hello world program
    .ORIG x3000
    LEA R0, HW
    PUTS
    ;; halt the program
    HALT
    ;; other data
HW  .STRINGZ "hello world!"
    .END
