#include "utils.h"
#include "timing.h"
#include "kemal_util.h" // ADDED TO TEST SORTING ALGORITHM

uint32_t	ticks;

int	ft_cmp(int a, int b)
{
	return (a < b);
}

int myMain(int size, int *arr, t_list *area)
{
	uint32_t	time;
	uint32_t	time_asm;
	t_list		*aux;
	int				num1;
	int				num2;
	int				s;
	int is_sorted, is_sorted_asm;
	
	for (s = 5; s < size; s += 5)
	{
		ft_lstfill(area, arr, s);
		Systick_Start();
		aux = area;
		ft_lstsort(&aux, ft_cmp);
		time = Systick_Stop();

		// check if the function correctly sorted the linked list.
		is_sorted = check_if_linked_list_sorted(aux);

		ft_lstfill(area, arr, s);
		Systick_Start_asm();
		aux = area;
		ft_lstsort_asm(&aux, ft_cmp);
		time_asm = Systick_Stop_asm();

		// check if the function correctly sorted the linked list.
		is_sorted_asm = check_if_linked_list_sorted(aux);
	}

	(void)time;
	(void)time_asm;
	ft_lstfill(area, arr, size);
	num1 = ft_lstlast(area)->num;
	num2 = ft_lstlast_asm(area)->num;
	(void)num1;
	(void)num2;
	return (0);
}
