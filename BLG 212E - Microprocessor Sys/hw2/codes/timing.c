#include "timing.h"

extern uint32_t	ticks;

// you should first remove this function and write it in timing_asm.s as assembly function.
// void SysTick_Handler(void)
// {
// 	// Increment the tick counter every time SysTick interrupt occurs
// 	ticks++;
// }

void Systick_Start(void)
{
	ticks = 0;

	SysTick->LOAD = (SystemCoreClock / 100000) - 1; // 1us interval
	SysTick->VAL = 0U;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

uint32_t	Systick_Stop(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	uint32_t	elapsed = ticks; // 10s of us
	return (elapsed);
}
