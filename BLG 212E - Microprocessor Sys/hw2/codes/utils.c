#include "utils.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new)
		return ;
	if (!lst)
	{
		*lst = new;
		return ;
	}
	new->next = *lst;
	*lst = new;
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*aux;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	aux = *lst;
	while (aux->next)
		aux = aux->next;
	aux->next = new;
}

int	ft_lstsize(t_list *lst)
{
	int	c;

	c = 0;
	while (lst)
	{
		lst = lst->next;
		c++;
	}
	return (c);
}

void	ft_lstfill(t_list *area, int *arr, int size)
{
		int			i;
	t_list	*aux;

	aux = area;
	aux->num = arr[0];
	aux->next = 0;
	i = 1;
	while (i < size)
	{
		aux = &area[i];
		aux->num = arr[i];
		aux->next = 0;
		ft_lstadd_back(&area, aux);
		i++;
	}
}