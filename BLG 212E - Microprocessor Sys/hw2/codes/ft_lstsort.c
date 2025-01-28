#include "utils.h"

void	ft_lstsplit(t_list *source, t_list **front, t_list **back)
{
	t_list	*slow;
	t_list	*fast;

	if (!source || !source->next)
	{
		*front = source;
		*back = 0;
		return;
	}
	slow = source;
	fast = source->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*front = source;
	*back = slow->next;
	slow->next = 0;
}

t_list	*ft_lstmerge(t_list *a, t_list *b, int (*f_comp)(int, int))
{
	t_list	*result = 0;

	if (!a)
		return (b);
	if (!b)
		return (a);

	if (f_comp(a->num, b->num))
	{
		result = a;
		result->next = ft_lstmerge(a->next, b, f_comp);
	}
	else
	{
		result = b;
		result->next = ft_lstmerge(a, b->next, f_comp);
	}
	return (result);
}

void	ft_lstsort(t_list **lst, int (*f_comp)(int, int))
{
	t_list	*head = *lst;
	t_list	*front;
	t_list	*back;

	if (!head || !head->next)
		return;

	ft_lstsplit(head, &front, &back);
	ft_lstsort(&front, f_comp);
	ft_lstsort(&back, f_comp);
	*lst = ft_lstmerge(front, back, f_comp);
}
