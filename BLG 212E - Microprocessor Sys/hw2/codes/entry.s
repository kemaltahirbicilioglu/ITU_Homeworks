lstSize EQU		800
		AREA	List_Area, DATA, READWRITE		;Define this part will write in data area
		ALIGN	
lst_ar	SPACE	lstSize						;Allocate space from memory for y
lst_end
		AREA	Sorting_Algo, code, readonly		;Define this part will write as code
		ENTRY
		THUMB
		ALIGN 
		EXPORT	__main
		EXTERN	myMain
__main	FUNCTION
		LDR	R0, =x_end
		LDR R1, =x_array
		SUBS R0, R0, R1
		ASRS R0, #2
		LDR R2, =lst_ar
		PUSH {R0}
		PUSH {R1}
		PUSH {R2}
		BL	myMain
stop	B	__main; instead stopping, repeat everything									;Infinite loop
		ALIGN
		ENDFUNC
x_array DCD	52, 25, 70, 12, 46, 97, 5, 62, 41, 98, 71, 81, 24, 56, 49, 50, 88, 91, 27, 80, 68, 15, 58, 30, 77, 69, 23, 57, 4, 22, 39, 75, 72, 20, 1, 82, 43, 54, 96, 11, 21, 19, 10, 73, 55, 31, 45, 59, 8, 79, 67, 40, 48, 85, 78, 18, 35, 44, 60, 38, 100, 13, 89, 95, 6, 29, 7, 74, 26, 61, 66, 37, 86, 99, 42, 9, 87, 3, 53, 83, 64, 90, 14, 2, 36, 76, 63, 94, 16, 51, 28, 93, 32, 65, 33, 92, 84, 47, 17, 34, 42
x_end
c_timer_start DCD 0
c_timer_stop DCD 0
		END
