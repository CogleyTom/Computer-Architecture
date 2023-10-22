.global _start

_start:

	MOV R0, # 1        @ File descriptor (1 for stdout)
	MOV R1, # 10       @ Put the value 10 into R1
	MOV R2, # 16       @ Put the value 16 into R2
	LDR R3, =message   @ Pointer to the message
	LDR R4, =len       @ message length
	MOV R7, #4         @ syscall number for sys_write
	B _loop            @ branch to loop function
	
_loop:
	CMP R2, R1     @ Compare R1 to R2
	BEQ _finish     @ If they are equal branch to finish label
	ADD R1, #3     @ Add R0 to R1
	B _loop         @ Branch back and re-run loop
	
_finish:
	MOV r7, #1         @ syscall number for sys_exit
	SWI 0              @ invoke syscall
	
.data
message:
	.asciz "Program is done!\n"
len = . - message
	
	
	