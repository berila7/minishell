/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:48:20 by anachat           #+#    #+#             */
/*   Updated: 2025/06/25 09:49:48 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*copy_node(t_gcnode **gc, t_env *src)
{
	t_env	*new;

	new = gc_malloc(gc, sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = gc_strdup(gc, src->key);
	new->value = gc_strdup(gc, src->value);
	new->next = NULL;
	return (new);
}

static void	swap_env(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_val;

	tmp_key = a->key;
	tmp_val = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_val;
}

static void	sort_env(t_env *env)
{
	t_env	*curr;
	int		sorted;

	if (!env)
		return ;
	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		curr = env;
		while (curr && curr->next)
		{
			if (ft_strcmp(curr->key, curr->next->key) > 0)
			{
				swap_env(curr, curr->next);
				sorted = 0;
			}
			curr = curr->next;
		}
	}
}

static void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

void	sorted_env(t_gcnode **gc, t_env *env)
{
	t_env	*copy;
	t_env	*curr;
	t_env	*new;

	copy = NULL;
	curr = NULL;
	while (env)
	{
		new = copy_node(gc, env);
		if (!copy)
			copy = new;
		else
			curr->next = new;
		curr = new;
		env = env->next;
	}
	sort_env(copy);
	print_env(copy);
	free_env(gc, copy);
}
