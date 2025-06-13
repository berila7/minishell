/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_fail.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:34:27 by anachat           #+#    #+#             */
/*   Updated: 2025/06/13 16:15:33 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_pid_to_list(t_data *data, pid_t pid)
{
	t_pid_node	*new;

	new = gc_malloc(&data->gc, sizeof(t_pid_node));
	new->pid = pid;
	new->next = data->pids;
	data->pids = new;
}

void	kill_all_pids(t_data *data)
{
	t_pid_node	*curr;
	t_pid_node	*next;

	curr = data->pids;
	while (curr)
	{
		kill(curr->pid, SIGINT);
		next = curr->next;
		gc_free(&data->gc, curr);
		curr = next;
	}
	data->pids = NULL;
}
