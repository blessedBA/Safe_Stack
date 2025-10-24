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
PUSH 2500
POPR RBX ; граница цикла
POPR RAX ; наш счетчик цикла
PUSH 35 ; ASCII код '#'
PUSH 42 ; ASCII код '*'
POPR RHX
POPR RGX
PUSH 50
PUSH 50
POPR REX ; ширина рисунка
POPR RFX ; длина рисунка

:2
CALL :3
PUSHR RAX
PUSHR REX
DIV
POPR RCX ; посчитали y
CALL :5 ; посчитали х
POPR RDX
CALL :6
CALL :7
ADD
PUSH 200 ; радиус нашего кружочка
JA :8
PUSHR RHX  ; достаем аски код звездочки
POPM [RAX] ; сохраняем по i-ому индексу
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
PUSHR REX
DIV
PUSHR REX
MUL
SUB
RET


:6 ; посчитать (y - 25)^2
PUSHR RCX
PUSH 2
MUL
PUSH 25
SUB
PUSHR RCX
PUSH 2
MUL
PUSH 25
SUB
MUL
RET


:7 ; посчитать (x - 25)^2
PUSHR RDX
PUSH 25
SUB
PUSHR RDX
PUSH 25
SUB
MUL
RET


:8 ; условие - внутри ли круга? (выполнение в случае правды)
PUSHR RGX ; достаем аски код решеточки
POPM [RAX] ; кладем его по i-ому индексу в оперативку
CALL :1
JMP :2


:9
HLT
