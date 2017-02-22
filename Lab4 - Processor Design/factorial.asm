
addi $v0, $0, 0		# Return register
addi $a0, $0, 2		# n
addi $a1, $0, 1		# i
addi $v1, $0, 0		# return for multiply
addi $a2, $v1, 1

beq $a0, $0, basecase	# n == 0
add  $0, $0, $0			# NOP

addi $at, $0, 1		# n == 1
beq $a0, $at, basecase
add  $0, $0, $0			# NOP

loop:
	beq  $a1, $a0, stop
	add  $0, $0, $0	# NOP

	add $a2, $a2, $v1
	addi $a3, $a1, 1
	addi $v1, $0, 0
	# Multiplies $a2 with $a3 and returns in $v1
	multiply:
		beq  $a2, $0, done
		add  $0, $0, $0	# NOP
		
		add	 $v1, $v1, $a3
		addi $a2, $a2, -1

		beq  $0, $0, multiply
		add  $0, $0, $0	# NOP
	
	done:
		addi $a1, $a1, 1	# i++
		
		beq  $0, $0, loop
		add  $0, $0, $0		# NOP

basecase:
	addi $v1, $0, 1
	beq  $0, $0, stop
	add  $0, $0, $0			# NOP

stop:
	add  $v0, $v0, $v1		# Increment global return
	beq  $0,$0, end

end:
	add  $0, $0, $0			# NOP
	#beq $0, $0, end		# NOP