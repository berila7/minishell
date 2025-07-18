/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:05 by anachat           #+#    #+#             */
/*   Updated: 2025/06/17 10:35:17 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_if_exit(char *name, int status, t_gcnode **gc)
{
	if (status != 999)
	{
		exit_status(status, 1);
		if (equal(name, "exit"))
		{
			gc_free_all(gc);
			exit(status);
		}
	}
	else
		exit_status(1, 1);
}

void	exec_builtin(t_cmd *cmd, t_data *data, int exit_flag)
{
	int		ext_st;
	char	*name;

	ext_st = 0;
	name = cmd->args[0];
	if (equal(name, "echo"))
		ext_st = ft_echo(cmd->args);
	else if (equal(name, "cd"))
		ext_st = ft_cd(cmd->args, data);
	else if (equal(name, "pwd"))
		ext_st = ft_pwd(data);
	else if (equal(name, "export"))
		ext_st = ft_export(cmd->args, data);
	else if (equal(name, "unset"))
		ext_st = ft_unset(cmd->args, data);
	else if (equal(name, "env"))
		ext_st = ft_env(cmd, data->env);
	else if (equal(name, "exit"))
		ext_st = ft_exit(cmd->args, exit_flag);
	dup2_og(data);
	check_if_exit(name, ext_st, &data->gc);
}
