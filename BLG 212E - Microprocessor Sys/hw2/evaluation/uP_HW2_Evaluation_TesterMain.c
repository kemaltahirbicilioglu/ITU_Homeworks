#include "utils.h"
#include "timing.h"

uint32_t	ticks;

int	ft_cmp(int a, int b)
{
	return (a > b);
}

int abs(int n)
{
	return (n >= 0 ? n :-n);
}
 
int ft_cmp2(int a, int b)
{
	int ah = a / 10;
	int al = a % 10;
	int bh = b / 10;
	int bl = b % 10;
	
	if (ah > bh)
		return 1;
	if (ah == bh && al < bl)
		return 1;
	return 0;
}
// n, n + 1, ..., n + 9,
// ...,
// 20, 21, 22, ..., 29,
// 10, 11, 12, ..., 19,
// 0, 1, 2, ..., 9.

int ft_cmp3(int a, int b)
{
	return abs(a) < abs(b);
}
// n+9,-(n+8), (n+8), ..., 5,-4, 3, 2,-1, 0.

int myMain(int size, int *arr, t_list *area)
{
	(void)size;
	t_list	*aux;
	int			len;

	ft_lstfill(area, arr, 100);
	aux = area;
	ft_lstsort_asm(&aux, ft_cmp);
	len = ft_lstsize(aux);

	ft_lstfill(area, arr, 100);
	aux = area;
	ft_lstsort_asm(&aux, ft_cmp2);
	len = ft_lstsize(aux);

	ft_lstfill(area, arr, 100);
	aux = area;
	ft_lstsort_asm(&aux, ft_cmp3);
	len = ft_lstsize(aux);

	(void)len;
	return (0);
}
