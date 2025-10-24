; метка 1 - инкремент счетчика
; метка 2 - цикл
; метка 3 - проверка условия цикла
; метка 4 - получить y
; метка 5 - получить x
; метка 6 - посчитать (y - 5)^2
; метка 7 - посчитать (x - 5)^2
; метка 8 - условие - внутри ли круга?
; метка 9 - КОНЕЦ


PUSH 0
PUSH 100
POPR RBX ; граница цикла
POPR RAX ; наш счетчик цикла
PUSH 1
PUSH 0
POPR RHX ; индекс в оперативке на символ '*'
POPR RGX ; индекс в оперативке на символ '#'
PUSH 35 ; ASCII код '#'
POPM [RGX]
PUSH 42 ; ASCII код '*'
POPM [RHX]


:2
CALL :3
PUSHR RAX
PUSH 10
DIV
POPR RCX ; посчитали y
CALL :5 ; посчитали х
POPR RDX
CALL :6
CALL :7
ADD
PUSH 3
JA :8
PUSHM [RHX]
OUT ; печать *
CALL :1
JMP :2


:1
PUSHR RAX ; сделать инкремент счетчика
PUSH 1
ADD
POPR RAX ; new
RET


:3 ; - проверка условия цикла
PUSHR RAX
PUSHR RBX
JBE :9
RET


:5 ; посчитать x
PUSHR RAX
PUSHR RAX
PUSH 10
DIV
PUSH 10
MUL
SUB
RET


:6 ; посчитать (y - 5)^2
PUSHR RCX
PUSH 5
SUB
PUSHR RCX
PUSH 5
SUB
MUL
RET


:7 ; посчитать (x - 5)^2
PUSHR RDX
PUSH 5
SUB
PUSHR RDX
PUSH 5
SUB
MUL
RET


:8 ; условие - внутри ли круга? (выполнение в случае правды)
PUSHM [RGX]
OUT
CALL :1
JMP :2


:9
HLT
