/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:36:35 by anachat           #+#    #+#             */
/*   Updated: 2025/04/12 14:54:19 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cmd_path(char *cmd, t_env *env)
{
	t_env	*curr_env;
	char	*path;

	while (curr_env)
	{
		if (ft_strcmp(curr_env->key, "PATH") == 0)
		{
			path = ft_split(curr_env->value, ':');
		}
		curr_env = curr_env->next;
	}
}

static void	set_cmd_path(t_command *cmd, t_env *env)
{
	t_command	*curr_cmd;

	curr_cmd = cmd;
	while (curr_cmd)
	{
		if (curr_cmd->args && curr_cmd->args[0])
			curr_cmd->path = get_cmd_path(curr_cmd->args[0], env);
		curr_cmd = curr_cmd->next;
	}
	
}

int	exec(t_data *data)
{
	set_cmd_path();

	

	return (data->exit_status);
}