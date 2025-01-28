number_a EQU 51
number_b EQU 85
	
	AREA gcd, CODE, READONLY		; Start code section
	ENTRY
    THUMB
    ALIGN


; subroutines here.
find_gcd
	PUSH	{R1, R2, LR}				; push the parameters

while_loop
	CMP		R1, R2					; if a==b
	BEQ		gcd_return				; return a
	BL		subtract
	B		while_loop

subtract
	CMP		R1,	R2					; compare R1 R2
	BGE		R1_larger				; R1 is larger directly subtract otherwise swap them
	SUBS	R2, R2, R1				; R2 = R2-R1, b = b-a
	BX		LR						; return

R1_larger
	SUBS	R1, R1, R2				; R1 = R1-R2
	BX		LR						; return
	

gcd_return
	PUSH	{R1}
	POP		{R0, R1, R2, PC}		; R0 keeps the answer


__main	FUNCTION 
	EXPORT	__main
	; write your code here.
	LDR		R1, =number_a
	LDR		R2, =number_b
	BL		find_gcd
	

stop	
	B stop

	ALIGN	
    ENDFUNC								; End of main function			
			
x_array DCD 10
x_array_end
		END