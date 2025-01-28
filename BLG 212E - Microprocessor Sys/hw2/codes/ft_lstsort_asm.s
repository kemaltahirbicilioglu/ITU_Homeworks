; Function: ft_lstsort_asm
; Parameters:
;   R0 - Pointer to the list (address of t_list *)
;   R1 - Pointer to comparison function (address of int (*f_comp)(int, int))
        AREA    Sorting_Code, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  ft_lstsort_asm
		EXTERN	ft_lstsize				; get the function from utils.c
next	EQU		4

ft_lstsort_asm FUNCTION
		PUSH	{R0-R1, LR}
		;LDR	R3, [SP]	 ; you will see the first argument
								; of the function in R3
		;LDR	R3, [SP, #4] ; you will see the second argument
								; of the function in R3
		; Given a singly linked list,
		; sort it using bubble sort by swapping nodes

		LDR		R0, [SP]				; address of the aux
		LDR		R0, [R0]				; head address of the linked list 
		; find the size of the linked list using the function ft_lstsize defined in utils.c
		; it pushes the R0 as the parameter. head of the linked list is in R0
		BL		ft_lstsize				; call size function, will return the answer in R0
		; now R0 keeps the size of the linked list
		MOVS	R1, #1					; R1 will keep if a swapped occur during the inner loop
		MOVS	R2, #0					; previous node, initially null
		MOVS	R6, #0					; outer loop index
outer_loop								; loop always start from the head node
		CMP		R1, #0					; if any swapped happend
		BEQ		end						; no swap in the inner loop, finish sorting
		MOVS	R1, #0					; clear the swapped flag	
		CMP		R0, R6					; check if all the elements are traversed
		BEQ		end						; return, all elements traversed
		LDR		R3, [SP]				; &aux
		LDR		R3, [R3]				; R3 is the head node
		MOVS	R2, #0					; previous node, initially null, SHOULD BE CLEARED BEFORE INNER LOOP		
		MOVS	R7, R0					; find the time that inner loop should iterate
		SUBS	R7, R7, R6				; inner looop times = total size - outer loop index
		SUBS	R7, R7, #1				; inner loop times = inner loop times -1, since there is no next for the last node
inner_loop			
		PUSH	{R6, R7}				; do not lose the index information, now we are able to use R6,R7 for other purposes
		CMP		R7, #0					; check if inner loop is over
		BEQ		inner_loop_finish		; return from inner loop
		LDR		R5, [R3]				; value of the current node
		LDR		R4, [R3, #next]			; R4 keeps the next node
		LDR		R4, [R4]				; value of the next node
		LDR		R6, [SP, #12]			; get the cmp function pointer, since we pushed 2 registers, SP has changed
		PUSH	{R0-R5}					; push all the register for not losing the content here in the inner loop
		; R0 and R1 will be pushed as a parameter to the ft_cmp function
		MOVS	R0, R5					; value of the current node
		MOVS	R1, R4					; value of the next node
		BLX		R6						; call the ft_cmp 
		CMP		R0, #0					; if swap needed function returns 0
		POP		{R0-R5}					; pop back registers we are using 
		BEQ		swap_needed				; call swap needed
		; swap is not needed
		MOVS	R2, R3					; make previous as current for the next iteration
		LDR		R4, [R3, #next]			; R4 keeps the next node
		MOVS	R3, R4					; make current as next for the next iteration
		B		inner_loop_continue		; continue to the inner loop 

swap_needed
		MOVS	R1, #1					; update swap flag
		LDR		R4, [R3, #next]			; R4 keeps the next node
		LDR		R5, [R4, #next]			; keep the next->next
		STR		R3, [R4, #next]			; next->next = current
		STR		R5, [R3, #next]			; curr->next = next->next
		CMP		R2, #0					; check if prev is null
		BNE		prev_is_not_null		; call prev is not null
		; prev null means current node is the head node, we should update aux
		LDR		R7, [SP, #8]			; R7 keeps the address of the aux
		STR		R4, [R7]				; update head node of the linked list
		MOVS	R2, R4					; update previous node
		B		inner_loop_continue		; continue inner loop
prev_is_not_null
		STR 	R4, [R2, #next]			; prev->next = next node
		MOVS	R2, R4					; update previous
inner_loop_continue
		POP		{R6,R7}					; get the index information back
		SUBS	R7, R7, #1				; decrement the loops neede for the inner loop
		B		inner_loop				; continue to the inner loop again

inner_loop_finish
		POP		{R6,R7}					; get the index information back
		ADDS	R6, R6, #1				; increment the index
		B		outer_loop				; continue the loop

		
end		

		POP		{R0-R1, PC}
		ENDFUNC
		END
		