; initialization
		.ORIG x3000
		LEA R1, OrigLoc
		LDW R1, R1, #0		; R1 = x4000
		AND R2, R2, #0
		ADD R2, R2, #1
		STB R2, R1, #0
		ADD R2, R2, #1
		STB R2, R1, #1
		ADD R2, R2, #1
		STB R2, R1, #2
		ADD R2, R2, #1
		STB R2, R1, #3
		LEA R2, Mask
		LDB R2, R2, #0
		STB R2, R1, #4
		LEA R0, Chk 
		LDW R0, R0, #0		; R0 = x-4009
		ADD R2, R1, #4		; R2 = x4004
		ADD R4, R1, #5		; R4 = x4005 (gets incremented)
		LDB R2, R2, #0		; R2 = control mask

; main loop of program
NextLoc	AND R3, R2, #3		; only bits 0 and 1 of current mask
		ADD R3, R1, R3		; R3 = address of relevant memory
		LDB R5, R3, #0
		STB R5, R4, #0

; preparation for next loop and checking if program is done
		RSHFL R2, R2, #2	; shift control mask 2 bits right
		ADD R4, R4, #1		; increment memory location to store to
		ADD R7, R0, R4
		BRn NextLoc			; if R4 is x4009, we're done
		ADD R1, R1, #10
		STW R1, R1, #0
		HALT
OrigLoc	.FILL x4000
Chk		.FILL x-4009
Mask    .FILL x1B
		.END
