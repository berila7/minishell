/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:01:58 by anachat           #+#    #+#             */
/*   Updated: 2025/05/16 21:03:23 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_gcnode	t_gcnode;

struct s_gcnode {
	void		*ptr;
	t_gcnode	*next;
};

void	free_gc(t_gcnode **gc)
{
	t_gcnode	*curr;
	t_gcnode	*next;

	if (!gc)
		return ;
	curr = *gc;
	while (curr)
	{
		next = curr->next;
		free(curr->ptr);
		free(curr);
		curr = next;
	}
	*gc = NULL;
}

static void	append_gc(t_gcnode **gc, void *ptr)
{
	t_gcnode	*node;
	t_gcnode	*last;

	node = malloc(sizeof(t_gcnode));
	if (!node)
	{
		free_gc(gc);
		exit(1);
	}
	node->ptr = ptr;
	node->next = NULL;
	if (!*gc)
		*gc = node;
	else
	{
		last = *gc;
		while (last->next)
			last = last->next;
		last->next = node;
	}
}

void	*gc_malloc(t_gcnode **gc, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		free_gc(gc);
		exit(1);
	}
	ft_bzero(ptr, size);
	append_gc(gc, ptr);
	return (ptr);
}
