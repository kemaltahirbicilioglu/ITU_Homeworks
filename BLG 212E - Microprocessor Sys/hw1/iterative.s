; KEMAL TAHİR BICILIOĞLU	150210083
SIZE      EQU 3							; Number of items
CAPACITY  EQU 50						; Maximum capacity
ARRAY_SIZE EQU SIZE * 4					; Item array size
DP_SIZE   EQU (CAPACITY + 1) * 4		; DP array size

    ; Define data areas for arrays
    AREA MyData, DATA, READWRITE		; Define a writable data area
	ALIGN
dp SPACE DP_SIZE						; Allocate space for dp array
dp_end

    AREA knapsack_iterative, CODE, READONLY		; Start code section
	ENTRY
    THUMB
    ALIGN


dp_init_loop							; To make sure dp array is initialized with 0
    STR		R0, [R3, R4]				; Store 0 in dp array at offset R4
    ADDS	R4, R4, #4                  ; Increment index by 4
    CMP		R4, R5                      ; Compare index with DP_SIZE
    BLT		dp_init_loop                ; Repeat until dp array is initialized

    ; Start the knapsack process
    MOVS	R4, #0                      ; Initialize item index (R4 = 0)

knapsack_loop_1
    MOVS	R5, #CAPACITY               ; Set R5 to the maximum weight capacity

knapsack_loop_2
    ; Inner loop runs in reverse order from CAPACITY to 0
    LDR		R6, [R2, R4]                ; Load weight of current item (weight_array[R4])

    ; Update dp[R5] = max(dp[R5], dp[R5 - item_weight] + item_profit)
    CMP		R6, R5                      ; Compare weight with current capacity R5
    BGT		skip_weight                 ; Skip if current item cannot be included

    SUBS	R0, R5, R6                  ; Calculate (capacity - item_weight)
    LSLS	R0, R0, #2                  ; Scale to get dp address offset
    LDR		R0, [R3, R0]                ; Load dp[capacity - item_weight]
    LDR		R6, [R1, R4]                ; Load profit of current item
    ADDS	R0, R0, R6                  ; Add profit to dp value

    MOVS	R6, R5                      ; Load capacity into R6
    LSLS	R6, R6, #2                  ; Scale to get dp address offset
    LDR		R7, [R3, R6]                ; Load dp[capacity]

    CMP		R0, R7                      ; Compare dp[capacity] and calculated profit
    BLE		skip_weight                 ; Skip if no update is needed
    STR		R0, [R3, R6]                ; Update dp[capacity] with max value

skip_weight
    SUBS	R5, R5, #1                  ; Decrement weight capacity
    CMP		R5, #0                      ; Check if we've reached the end of dp
    BGE		knapsack_loop_2             ; Continue updating dp if within capacity

    ADDS	R4, R4, #4                  ; Move to the next item
    CMP		R4, #ARRAY_SIZE             ; Check if all items are processed
    BLT		knapsack_loop_1             ; Repeat for each item

    ; Load the final maximum profit value from dp[CAPACITY]
    MOVS	R5, #DP_SIZE                ; Move DP_SIZE into a register
    SUBS	R5, R5, #4                  ; Address of dp[CAPACITY]
    LDR		R0, [R3, R5]                ; Load the final maximum profit value
	B		stop

__main FUNCTION							; Start of main function
    EXPORT __main						; Export __main symbol for linking

    ; Load base addresses of the arrays
    LDR		R1, =profit_array			; Load base address of profit_array into R1
    LDR		R2, =weight_array			; Load base address of weight_array into R2
    LDR		R3, =dp						; Load base address of dp array into R3

    ; Initialize dp array to zero
    MOVS	R0, #0						; Set R0 to zero for initializing dp
    MOVS	R4, #0                      ; Set R4 to zero for the loop counter
    MOVS	R5, #DP_SIZE                ; Load DP_SIZE into R5
	B		dp_init_loop
	
stop
    B	stop							; Infinite loop to stop execution                        
	ALIGN	
    ENDFUNC								; End of main function                            


profit_array DCD 60,100,120				; Initialize the profit array				 
profit_array_end
weight_array DCD 10,20,30				; Initialize the weight array
weight_array_end
	END									; End of program                                
