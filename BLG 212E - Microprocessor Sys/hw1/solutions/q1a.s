W_Capacity	EQU 50							;#define W_Capacity 50
SIZE		EQU	3							;#define SIZE		3
	
			AREA data1, DATA, READONLY
profit		DCD	60, 100, 120				; profit array
endprofit
weight		DCD	10, 20, 30					; weight array
endweight





			AREA recursive, CODE, READONLY
			ENTRY
			THUMB
			ALIGN
__main		FUNCTION
			EXPORT	__main
			LDR		R0, =W_Capacity			; R0 stores W_capacity (the first argument)
			LDR		R1, =SIZE				; R1 stores	N (the second argument)
			BL		knapSack				; Call knapSack
loop		B		loop					; Infinite loop
	
	
	
			ENDFUNC
knapSack	FUNCTION
			CMP		R0, #0					; if n==0
			BEQ		ksRet					; return 0
			CMP		R1, #0					; if n==0
			BEQ		ksRet					; return 0
			B		kS1						; Otherwise continue
ksRet		MOVS	R0, #0					; R0 = 0
			BX		LR						; Return 0; R0 contains 0
kS1			LDR		R5, = weight			; R5 = weight
			SUBS	R1,	R1, #1				; R2 = n-1
			LSLS	R2, R1, #2				; R2 = R2*4, Weight array uses 4 byte for each value
			LDR		R3, [R5, R2]			; R3 <- weight[n-1]
			CMP		R3, R0					; if (weight[n-1] > W)
			BLE		kS2						; If less equal continue to kS2
			PUSH	{LR}					; Store return address
			BL		knapSack				; knapSack(W, n-1) -> R0 = W, R1 = n-1
			POP		{PC}					; Return using stack.
kS2			PUSH	{LR}					; Store return address
			PUSH	{R0,R1, R2, R3, R5} 	; Store will be used register, to protect
			BL		knapSack				; knapSack(W, n-1) -> R0 = W, R1 = n-1
			MOV     R7, R0					; R7 = knapSack (W,n-1)
			POP		{R0, R1, R2, R3, R5}	; Restore registers
			SUBS	R0, R3					; R1 = W - weight[n-1]
			PUSH	{R2, R5}				; Store will be used register, to protect
			BL		knapSack				; knapSack(W, n-1) -> R0 = W, R1 = n-1
			MOV     R6, R0					; R7 = knapSack (W - weight[n-1],n-1)
			POP		{R2, R5}				; Restore registers
			LDR		R5, =profit				; R5 = profit
			LDR		R3, [R5, R2]			; R3 <- profit[n-1]
			ADDS	R1, R6, R3				; R1 = profit[n-1] + knapSack (W - weight[n-1],n-1)
			MOVS	R0, R7					; R1 = knapSack (W,n-1)
			BL		max						; Return max(R0,R1)
			POP		{PC}					; Return usign stack


			ENDFUNC
			
max			FUNCTION
			CMP		R0, R1					; if (R0>R1)
			BGE 	max_r					; if true return R0
			MOVS 	R0, R1					; otherwise return R1
max_r		BX		LR
	
			ENDFUNC
			END
	
	
	
			
	