
FIND_FIB EQU 9
SIZE	EQU	 FIND_FIB + 1

	AREA MyData, DATA, READWRITE		; Define a writable data area
	ALIGN
memo SPACE SIZE*4					; Allocate space for dp array
memo_end

	AREA gcd, CODE, READONLY		; Start code section
	ENTRY
    THUMB
    ALIGN


; subroutines here.

initialize_memo
	LDR 	R7, =-1					; initialize with -1
	LDR		R1, =memo				; get the starting address of the array
	LDR		R2, =memo_end			; finishing address
loop
	CMP		R1, R2					; check if all traversed
	BEQ		initialize_memo_return	; return
	STR		R7, [R1]
	ADDS	R1, R1, #4				; increment the address
	B		loop

initialize_memo_return
	BX		LR

check_memo
	LDR		R7, =memo				; starting address of the memo
	MOVS	R6, R2					; R2 = R6
	LSLS	R6, #2					; offset
	LDR		R6, [R7, R6]			; R6 = memo[R2]
	LDR 	R5, =-1					; R5 keeps -1
	CMP		R6, R5					; is this calculated before
	BEQ		check_memo_return		; not calculated return
	; calculated before we can directly push the answer onto the stack then return
	MOVS	R0, R6					; R0 keeps the answer for fib(R2)
	PUSH	{R0}					; push the answer
	B		fibonacci_return		; return directly

check_memo_return
	BX		LR						; return

save_answer_in_memo
	LDR		R7, =memo				; starting address of the memo
	MOVS	R6, R2					; R2 = R6
	LSLS	R6, #2					; offset
	LDR		R5, [R7, R6]			; R6 = memo[R2]
	LDR 	R4, =-1					; R5 keeps -1	
	CMP		R5, R4					; is this saved before
	BNE		save_answer_in_memo_return		; already saved the answer
	STR		R0, [R7, R6]			; save the answer in the memo

save_answer_in_memo_return
	BX		LR

fibonacci	
	PUSH	{R2, LR}				; R1 is the current num
	
	BL		check_memo				; check if we previously find the answer for fib(R2)
	; base cases
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
	B		fibonacci_return				; return from the function

base_case_0
	MOVS	R0, #0					; answer is zero in this base case
	PUSH	{R0}					; push the answer
	B		fibonacci_return				; return

base_case_1
	MOVS	R0, #1					; if R1 = 1, return 1
	PUSH	{R0}					; push the answer
	B		fibonacci_return				; return

fibonacci_return
	BL		save_answer_in_memo		; whenever we are in fibonacci return it means we found an answer for fib(R2), store that
	POP		{R0,R1,PC}				; return from the function

__main	FUNCTION 
	EXPORT	__main
	; write your code here.
	BL	initialize_memo				; initializes the array with -1 since fib cannot be negative for any value

	LDR		R2, =FIND_FIB			; parameter for the initial call
	BL		fibonacci				; call fibonacci
	LDR		R1, =memo				; lastly R1 keeps the starting address of the memo

	
stop	
	B stop

	ALIGN	
    ENDFUNC								; End of main function			
	
	END
