		AREA  LstLast, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  ft_lstlast_asm
next	EQU		4

ft_lstlast_asm FUNCTION
		PUSH	{R0, LR}

		LDR		R1, [SP]
		MOVS	R0, #0
		CMP		R1, #0
		BEQ		end
			
loop	LDR		R0, [SP]
		LDR		R1, [R0, #next]
		CMP		R1, #0
		BEQ		end
		STR		R1, [SP]
		B		loop
end
		POP		{R0, PC}
		ENDFUNC
		END
