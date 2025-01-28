; KEMAL TAHİR BICILIOĞLU	150210083
SIZE      EQU 3							; Number of items
CAPACITY  EQU 50						; Maximum capacity
ARRAY_SIZE EQU SIZE * 4					; Item array size

    AREA knapsack_recursive, CODE, READONLY		; Start code section
	ENTRY
    THUMB
    ALIGN
		
knapsack
	PUSH	{R3,R4,LR}					; Push: current_index, current_weight, LR
	CMP		R3, #0						; check n == 0
	BEQ		zero_case					; if n == 0 return 0
	CMP		R4, #0						; check W == 0
	BEQ		zero_case					; if W == 0 return 0
	
	LDR		R2, =weight_array			; Get the starting address of the array
	MOVS	R7, R3						; Do not lose the index in this scope, move to R7
	SUBS	R7, R7, #1					; N = N-1
	LSLS	R7, R7, #2					; Find the address of N-1
	LDR		R7, [R2, R7]				; R7 = weights[N-1]
	
	CMP 	R7, R4						; weight[N-1] > W
	BGT		cannot_take_item			; Capacity is not enough to take the item
	
	; We can take the current item, we should consider both the cases where we do not take and we take. Choose the largest one when returning
	BL		not_take_item				; Do not take the item
	POP		{R3,R4}						; Get the values in this scope
	PUSH	{R0,R3,R4}					; Push them back, also with the return value of not taking the current item
	
	BL		take_item					; Take the current item
	POP		{R2,R3,R4}					; Get the values in this scope, R2 keeps return of not_take_item, R0 keeps return of take_item
	PUSH	{R3,R4}						; Push the values back
	
	LDR		R1, =profit_array			; Get the starting address of the array
	MOVS	R7, R3						; Do not lose the index in this scope, move to R7
	SUBS	R7, R7, #1					; N = N-1
	LSLS	R7, R7, #2					; Find the address of N-1
	LDR		R7, [R1, R7]				; R3 = profits[N-1]
	ADDS	R0, R0, R7					; R0 = take_item + profits[n-1]
	
	BL	find_max						; Find the maximum of take_item + profit[n-1] and not_take_item
	PUSH	{R0}						; Push the return value
	B function_return					; Return

find_max  ; returns larger of two numbers
	CMP		R0, R2						; Compare R0 and R2
	BGT		max_R0						; If R0 > R2 no need to change R0
	MOVS	R0, R2						; Load R2 to R0 since it is larger
max_R0
	BX		LR							; Return the max value in R0
		
		
zero_case
	MOVS	R0,#0;						; To return 0
	PUSH	{R0}						; Push 0 to stack, meaning that R0 will be 0 when function is returned
	B function_return					; Return
	
	
not_take_item
	SUBS	R3, R3, #1					; N = N-1
	B		knapsack					; Call knapsack
	
	
cannot_take_item						; We cannot take the item because capacity is not enough
	SUBS	R3, R3, #1					; N = N-1
	BL		knapsack					; Call knapsack
	PUSH	{R0}						; Push the return value, our return value is what the knapsack returned here.
	B		function_return				; Return from the function
	
	
take_item
	LDR		R2, =weight_array			; Get the starting address of the array
	MOVS	R7, R3						; Do not lose the index in this scope, move to R7
	SUBS	R7, R7, #1					; N = N-1
	LSLS	R7, R7, #2					; Find the address of N-1
	LDR		R7, [R2, R7]				; R7 = weights[N-1]
	SUBS	R4, R4, R7					; W - weights[n-1]
	SUBS	R3, R3, #1					; N = N-1
	B		knapsack					; Call knapsack
	
	
function_return
  POP  {R0,R3,R4, PC}					; R0 keeps answer


__main FUNCTION							; Start of main function
    EXPORT __main						; Export __main symbol for linking

	; R3 will always be N, R4 will always be W as the parameter of the function
    MOVS	R3, #SIZE                   ; Load n to R4
	MOVS	R4, #CAPACITY				; Load W to R3
	
	BL		knapsack					; branch to knapsack
	; When returned, return value will be stored in the R0
	LDR		R1, =profit_array 			; load profit array address in R1
	LDR		R2, =weight_array 			; load weight array address in R2
stop
    B	stop							; Infinite loop to stop execution                        
	ALIGN	
    ENDFUNC								; End of main function                            


profit_array DCD 60,100,120				; Initialize the profit array
profit_array_end
weight_array DCD 10,20,30				; Initialize the weight array
weight_array_end
	END									; End of program                                
