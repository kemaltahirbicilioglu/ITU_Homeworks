; This code copies the array x to the array y

array_size EQU 0x28
	
	AREA My_Array, DATA, READWRITE
	ALIGN

y_array	SPACE array_size
y_end

	AREA copy_array, code, readonly
	ENTRY
	THUMB
	ALIGN
		
array_copy
	MOVS	R1, #0					; index to traverse
	
array_copy_loop
	CMP		R1, #array_size			; if all elements are traversed
	BEQ		return_array_copy		; return
	LDR		R3, =x_array			; starting address of the array
	LDR		R4, [R3, R1]			; R2 = x_array[index]
	LDR		R3, =y_array			; starting address of the array
	STR		R4,	[R3,R1]				; copy the element, y_array[index] = x_array[index]
	ADDS	R1, #4					; increment the index
	B		array_copy_loop			; continue the loop
		

return_array_copy
	BX LR						; return from the array_copy

__main	FUNCTION 
	EXPORT	__main
	
	BL	array_copy
	B	stop

stop	
	B stop
			
			
x_array DCD 12, 20, 25, 60, 15, 53, 17, 65, 22, 1
x_array_end
		END