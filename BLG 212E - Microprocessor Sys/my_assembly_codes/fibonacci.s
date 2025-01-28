
FIND_FIB EQU 8

	AREA gcd, CODE, READONLY		; Start code section
	ENTRY
    THUMB
    ALIGN


; subroutines here.

fibonacci	
	PUSH	{R2, LR}				; R1 is the current num

	CMP		R2, #0					; compare if zero, base case
	BEQ		base_case_0				; go to base case 0

	CMP		R2, #1					; compare if one, base case
	BEQ		base_case_1				; go to base case 1

	; number is greater than 1
	SUBS	R2, #1					; num = num-1
	BL		fibonacci				; recursively call for num-1
	POP		{R2}					; we got the fib(num-1) answer in R0, and also get the context in this scope
	PUSH	{R0, R2}				; do not lose the context here, also push the answer
	SUBS	R2, #2					; num = num-2
	BL		fibonacci				; call for fib(num-2)
	POP		{R1, R2}				; R1 keeps the answer of fib(num-1) R2 is the context 
	PUSH	{R2}					; push context
	ADDS	R0, R0, R1				; sum the answers of fib(num-1) and fib(num-2)
	PUSH	{R0}					; push the answer
	B		fib_return				; return from the function

base_case_0
	MOVS	R0, #0					; answer is zero in this base case
	PUSH	{R0}					; push the answer
	B		fib_return				; return

base_case_1
	MOVS	R0, #1					; if R1 = 1, return 1
	PUSH	{R0}					; push the answer
	B		fib_return				; return

fib_return
	POP		{R0,R1,PC}				; return from the function

__main	FUNCTION 
	EXPORT	__main
	; write your code here.
	LDR		R2, =FIND_FIB
	BL		fibonacci

	
stop	
	B stop

	ALIGN	
    ENDFUNC								; End of main function			
