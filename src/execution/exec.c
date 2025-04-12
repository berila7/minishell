/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/04/12 13:56:37 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_cmd_path(t_command *cmd, t_env *env)
{
	t_command	*curr_cmd;

	curr_cmd = cmd;
	while (curr_cmd)
	{
		if (curr_cmd->args[0])
			curr_cmd->path = NULL;
		curr_cmd = curr_cmd->next;
	}
	
}

int	exec(t_data *data)
{
	set_cmd_path();

	

	return (data->exit);
}