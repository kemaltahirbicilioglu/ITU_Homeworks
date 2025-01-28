#ifndef TIMING_H
# define TIMING_H
#include "RTE_Components.h"
#include CMSIS_device_header

void			Systick_Start(void);
uint32_t	Systick_Stop(void);

void			Systick_Start_asm(void);
uint32_t	Systick_Stop_asm(void);

#endif