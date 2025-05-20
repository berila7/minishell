/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:01:58 by anachat           #+#    #+#             */
/*   Updated: 2025/05/20 17:18:25 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void	print_gc_nodes(t_gcnode *gc) {
	int i = 0;
	t_gcnode *curr = gc;
	while (curr)
	{
		printf("Node [%d]: value: ptr = %p\n", i, curr->ptr);
		curr = curr->next;
		i++;
    }
}

void	gc_free(t_gcnode **gc, void *ptr)
{
	t_gcnode	*curr;
	t_gcnode	*prev;

	curr = *gc;
	prev = NULL;
	while (curr)
	{
		if (curr->ptr == ptr) {
			if (prev)
				prev->next = curr->next;
			else
				*gc = curr->next;
			if (curr->ptr)
				free(curr->ptr);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	free(ptr);
}

void	gc_free_all(t_gcnode **gc)
{
	t_gcnode	*curr;
	t_gcnode	*next;

	if (!gc	|| !*gc)
		return ;
	print_gc_nodes(*gc);
	curr = *gc;
	while (curr)
	{
		next = curr->next;
		if (curr->ptr)
			free(curr->ptr);
		free(curr);
		curr = next;
	}
}

static void	append_gc(t_gcnode **gc, void *ptr)
{
	t_gcnode	*node;
	t_gcnode	*last;

	node = malloc(sizeof(t_gcnode));
	if (!node)
	{
		print_err("Allocation Error\n", NULL);
		gc_free_all(gc);
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
		gc_free_all(gc);
		exit(1);
	}
	ft_bzero(ptr, size);
	append_gc(gc, ptr);
	return (ptr);
}

