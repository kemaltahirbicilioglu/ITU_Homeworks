
SIZE EQU 7

	AREA MyData, DATA, READWRITE		; Define a writable data area
	ALIGN
x_array SPACE SIZE*4					; Allocate space for dp array
x_array_end

	AREA gcd, CODE, READONLY		; Start code section
	ENTRY
    THUMB
    ALIGN


; subroutines here.

swap
	CMP		R5, R4					; check R5 > R4
	BGE		R5_bigger
	MOVS	R2,	R5					; do not lose R5
	MOVS	R5, R4					; swap
	MOVS	R4,	R2					; R4 = R5

R5_bigger
	BX		LR

store_swapped
	MOVS	R2, R1					; R2 = j
	LSLS	R2, #2					; R2 = R2*4, offset
	LDR		R3, =x_array			; starting address of the array
	STR		R4, [R3, R2]			; array[j] = R4 
	ADDS	R2, #4					; next index
	STR		R5,	[R3, R2]			; array[j+1] = R5 
	BX		LR						; return

sort_array
	PUSH	{LR}
	LDR		R7, =SIZE				; keep the size
	MOVS	R0, #0					; index of the outer loop

outer_loop
	CMP		R0, R7					; if all the elements are traversed
	BEQ		sort_array_return		; return
	MOVS	R1, #0					; index of the inner loop

inner_loop
	MOVS	R6, R7					; R6 = size
	SUBS	R6, R6, R0				; R6 = size - i
	SUBS	R6, R6, #1				; R6 = size - i - 1
	CMP		R1, R6					; check if j < size-i
	BEQ		inner_loop_return		; return from innerloop
	MOVS	R2, R1					; R2 = j
	LSLS	R2, #2					; R2 = R2*4, offset
	LDR		R3, =x_array			; starting address of the array
	LDR		R4, [R3, R2]			; R4 = array[j]
	ADDS	R2, #4					; next index
	LDR		R5,	[R3, R2]			; R5 = array[j+1]
	BL		swap					; bigger one will be in R5
	BL		store_swapped			; store the swapped values
	ADDS	R1, #1					; increment j
	B		inner_loop 


inner_loop_return
	ADDS	R0, R0, #1				; increment the index
	B		outer_loop				; continue the loop
	
sort_array_return
	POP		{PC}

__main	FUNCTION 
	EXPORT	__main
	; write your code here.
	LDR		R0, =x_array				; copy the array
	LDR		R1, =init_values
	MOVS	R2, #7                     ; Number of elements
loop_init
	LDR		R3, [R1]
	ADDS	R1, #4
	STR		R3, [R0]
	ADDS	R0, #4
	SUBS	R2, #1
	BNE		loop_init

	BL		sort_array
	LDR		R0, =x_array			; starting adress of the array is in R0

stop	
	B stop

	ALIGN	
    ENDFUNC								; End of main function			

init_values     DCD    10, 5, 9, 12, 6, 7, 4
init_values_end
    END