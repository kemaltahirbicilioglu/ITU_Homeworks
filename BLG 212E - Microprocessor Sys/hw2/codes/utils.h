#ifndef UTILS_H
# define UTILS_H

typedef struct s_list
{
	int						num;  //4 bytes
	struct s_list	*next; // 4 bytes
}								t_list;

t_list	*ft_lstlast			(t_list *lst);
int			ft_lstsize			(t_list *lst);
void		ft_lstadd_front	(t_list **alst,	t_list *new_node);
void		ft_lstadd_back	(t_list **alst,	t_list *new_node);
void		ft_lstsort    	(t_list **lst, int (*f_comp)(int, int));
void		ft_lstfill			(t_list *area, int *arr, int size);


void		ft_lstsort_asm	(t_list **lst, int (*f_comp)(int, int));
t_list	*ft_lstlast_asm	(t_list *lst);

#endif