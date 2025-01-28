        AREA    Timing_Code, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  Systick_Start_asm
        EXPORT  Systick_Stop_asm
		EXPORT	SysTick_Handler ; When the correct time comes,
									; remove this from the comments.
		EXTERN	ticks

; Variables for SysTick addresses.
SYSTICK_CTRL    EQU     0xE000E010  ; CTRL address
SYSTICK_LOAD    EQU     0xE000E014  ; LOAD address
SYSTICK_VAL     EQU     0xE000E018  ; VAL address

; Variable for control value.
CTRL_VAL        EQU     0x7         ; CTRL value to be set

SysTick_Handler FUNCTION
		PUSH	{LR}                ; save the return address
		; You should only increment ticks here.
        LDR     R0, =ticks          ; address of the ticks variable
        LDR     R1, [R0]            ; current value of ticks
        ADDS    R1, R1, #1          ; increment the tick
        STR     R1, [R0]            ; store back the incremented value
		POP		{PC}                ; return
		ENDFUNC

Systick_Start_asm FUNCTION
		PUSH	{LR}                ; save the return address
		; You should initialize SysTick here.
        LDR     R0, =ticks          ; address of the ticks variable
        MOVS    R1, #0              ; initially ticks should be 0
        STR     R1, [R0]            ; store back the initialized ticks
        
        ; calculate LOAD value as (SystemCoreClock / 100000) - 1, system clock was 25Mhz
        LDR     R0, =SYSTICK_LOAD   ; address of the systick load register
        MOVS    R1, #249            ; directly load the value 249
        STR     R1, [R0]            ; store calculated LOAD value

        ; set the systic val register
        LDR     R0, =SYSTICK_VAL    ; address of the systic val register
        MOVS    R1, #0              ; initially it should be 0
        STR     R1, [R0]            ; store the value in systic val register
        
        ;set the CTRL register
        LDR     R0, =SYSTICK_CTRL   ; address of the SysTick CTRL register
        LDR     R1, =CTRL_VAL       ; enable SysTick with processor clock and interrupt
        STR     R1, [R0]            ; store systic CTRL value
        
        POP		{PC}                ; return
		ENDFUNC


Systick_Stop_asm FUNCTION
		PUSH	{LR}                ; save the return address
		; You should stop SysTick, zero the ticks,
			; and return "non-zero value of ticks".

        LDR     R0, =SYSTICK_CTRL   ; address of the SysTick CTRL register        
        LDR     R1, [R0]            ; read current CTRL value
        MOVS    R2, #0x1            ; mask for the ENABLE bit
        MVNS    R2, R2              ; invert the mask (0xFFFFFFFE)
        ANDS    R1, R1, R2          ; clear the ENABLE bit
        STR     R1, [R0]            ; store back to disable SysTick

        LDR     R0, =ticks          ; address of ticks variable
        LDR     R1, [R0]            ; load current ticks value
        MOV     R0, R1              ; move ticks value to R0 for return

		POP		{PC}                ; return
		ENDFUNC

        END