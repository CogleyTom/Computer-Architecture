

section .data
    var dd 20             ; creates 4 byte word in memory location var
    ArrayVar dd 10, 5, 25 ; creates array with three 4 byte words representing ints
    loadVar dd 12      ; creates "loadVar" memory location with 4 byte word 126

    
section .text

global main
main:
    mov ebp, esp; for correct debugging
    call firstFunc         ; Jumps to first function
    secondFunc:            ; Second function
    mov eax, 20 
    mov edx, 3
    idiv ebx                ; divides 20 by 3 placing 10 into ebx and 8 into edx
    ret                     ; returns to end to clean stack
    firstFunc:              ; First function
    mov ebp, esp; for correct debugging
    mov eax, [var]  ; puts 20 in eax
    mov ebx, [ArrayVar]  ; puts 10 in ebx
    mov ecx, [ArrayVar + 4] ; puts 5 in ecx
    mov edx, [ArrayVar + 8] ; puts 25 in edx
    add eax, ebx            ; adds content in ebx into eax equaling 30
    imul ecx, 3             ; multiplies value in ecx by 3 and puts in ecx equals 15
    decLoop:
    dec eax                 ; decrement eax by 1 until loop is met
    cmp eax, edx            ; compares value in eax to edx
    jne decLoop             ; if eax does not equal edx then jump to decLoop beginning
    push eax                ; push eax to stack
    minusLoop:
    sub ecx, 2              ; loop to subtract 2 from ecx starting at 15
    cmp ecx, 10
    jge minusLoop            ; Loop until value in ecx is less than 10
    push ecx                ; pushes ecx to top of stack which is 9
    pop eax                 ; pops top of stack value into eax equaling 9
    lea ecx, [loadVar]      ; loads value from address "loadVar" into register ecx
    add esp, 4              ; cleas up the stack using the stack pointer
    call secondFunc
    add esp, 4              ; cleans stack
    ret                     ; ends program