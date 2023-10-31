.data
array:   	.word  30, -12, -50, 48, 7, 19, -1030, 400, -32, 76
array_size:	.word	10
print_min:	.asciiz "minimum value: " 
print_max:	.asciiz "\nmaximum value: " 
.globl main
.text


main:
         la $a0, array					# load the address of array into $a0
         lw $a1, array_size				# $a1 is initially set to array size (10) and used as a counter
         lw $t2, ($a0)					# $t2 is the max value variable
         lw $t3, ($a0)					# $t3 is the min value variable
         
        loop:
        	beq $a1, $zero, printResults		# if the counter is now 0, entire array has been evaluated so jump to printResults
        	lw $t0, ($a0)				# $t0 initially points to array[0]
        	bge $t0, $t3, checkIfMin		# if $t0 >= $t3, check if $t0 is minimum value
        	move $t3, $t0
        	
        	checkIfMin:
        	ble $t0, $t2, checkIfMax		# if $t0 <= $t2, check if $t0 is max value
        	move $t2, $t0				
        	
        	checkIfMax:
        	subi $a1, $a1, 1			# decrease the array counter
        	addi $a0, $a0, 4
        	j loop					# jump to the beginning of the loop
        	
         printResults:
         	li $v0, 4				
         	la $a0, print_min			# print "minimum value" string
         	syscall
         
         	li $v0, 1
         	move $a0, $t3    			# print minimum value
         	syscall
         	
         	li $v0, 4
         	la $a0, print_max			# print "maximum value" string
         	syscall
         	
         	li $v0, 1	 			# print maximum value
         	move $a0, $t2
         	syscall
         		
         	li $v0, 10				# exit program	
        	syscall
	
