
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



__main	FUNCTION 
	EXPORT	__main
	; write your code here.
	
stop	
	B stop

	ALIGN	
    ENDFUNC								; End of main function			

init_values     DCD    10, 5, 9, 12, 6, 7, 4
init_values_end
    END
