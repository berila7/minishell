/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:01:58 by anachat           #+#    #+#             */
/*   Updated: 2025/05/18 17:27:33 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
		if (curr->ptr)
		{	
			free(curr->ptr);
			curr->ptr = NULL;
		}
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
		print_err("Allocation Error\n", NULL);
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
		print_err("Allocation Error\n", NULL);
		free_gc(gc);
		exit(1);
	}
	ft_bzero(ptr, size);
	append_gc(gc, ptr);
	return (ptr);
}
