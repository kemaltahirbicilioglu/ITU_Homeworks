W_Capacity	EQU 50						;#define W_Capacity 50
SIZE		EQU	3						;#define SIZE		3
	
			AREA data1, DATA, READONLY
profit		DCD	60, 100, 120			; profit array
endprofit
weight		DCD	10, 20, 30				; weight array
endweight		
			
			AREA data2, DATA, READWRITE
dp			SPACE	(50+1)*4			; DP array


			AREA iterative, CODE, READONLY
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
			PUSH	{LR}					; Store return address
			PUSH	{R0}					; Store R0, it will be used at the end of the program
			MOV    	R8, R0					; R8 = W
			LDR		R2, =dp					; R2 = address of the dp
			MOVS	R3, #0					; i
			LSLS	R4,	R0, #2				; R4 = W*4 
			MOVS	R5, #0					; R5 = 0 for clear the array
ksLp		CMP		R3, R4					; if i>W*4
			BGT		ks						; if true start knapsack
			STR		R5, [R2, R3]			; Clear dp[i]
			ADDS	R3, #4					; R3+=4
			B		ksLp					; Jump ksLp to check
			
ks			ADDS	R2, R1, #1				; R2 = n+1
			MOVS	R3, #1					; R3 = i = 1
for1		CMP 	R3, R2					; if i>= n+1
			BGE		ksRet					; if true break the loop
			MOV		R4, R8					; R4 = W
for2		CMP		R4, #0					; if R4 <0
			BLT		endF1					; if true break the inner loop
			LDR		R0, =weight				; R0 = address of weight
			SUBS	R5, R3, #1				; R5 = i-1
			LSLS	R5, #2					; R5 = 4(i-1)
			LDR		R6, [R0, R5]			; R6 = weight[i-1] 
			CMP		R6, R4					; if R6 > R4
			BGT		endF2					; if true jump the operation
			LDR		R0, =profit				; R0 = address of profit
			LDR		R7, [R0, R5]			; R7 = profit [i-1]
			SUBS	R6, R4, R6				; R6 = w- weight[i-1]
			LSLS	R6, #2					; R6 = 4*(w- weight[i-1])
			LDR		R0, =dp					; R0 = address of dp
			LDR		R6, [R0, R6]			; R6 = dp[w-weight[i-1]]
			ADDS	R6, R7					; R6 = dp[w-weight[i-1]]+profit[i-1]
			LSLS	R7, R4, #2				; R1 = 4*w
			LDR		R1, [R0,R7]				; R1 = dp[w]
			MOVS	R0, R6					; R0 = dp[w-weight[i-1]]+profit[i-1]
			BL		max						; R0 = max (dp[w], dp[w-weight[i-1]]+profit[i-1])
			LDR		R1, =dp					; R1 = address of dp
			STR		R0, [R1,R7]				; dp[w] = max (dp[w], dp[w-weight[i-1]]+profit[i-1])
			
endF2		SUBS	R4, #1					; R4-=1, w--			
			B		for2					; Branch inner loop
						
endF1		ADDS	R3, #1					; R3 +=1 , i++
			B		for1					; Branch for loop
							
ksRet		POP		{R0}
			LDR		R2, =dp					; R2 = address of the dp
			LSLS	R0, R0, #2				; R0 = R0*4 (4 byte 1 index)
			LDR		R0, [R2, R0]			; R0 = dp[W]
			POP		{PC}					; Return usign stack
			
			ENDFUNC
			
max			FUNCTION
			CMP		R0, R1					; if (R0>R1)
			BGE 	max_r					; if true return R0
			MOVS 	R0, R1					; otherwise return R1
max_r		BX		LR
	
			ENDFUNC
			END
	
	
	
			
	