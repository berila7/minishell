/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:05 by anachat           #+#    #+#             */
/*   Updated: 2025/04/13 21:25:05 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	equal(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (0);
	if (ft_strcmp(s1, s2) == 0)
		return (1);
	return (0);
}

int	is_builtin(t_cmd *cmd)
{
	char	*name;

	name = cmd->args[0];
	if (equal(name, "echo") || equal(name, "cd") || equal(name, "pwd")
		|| equal(name, "export") || equal(name, "unset")
		|| equal(name, "env") || equal(name, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_data *data)
{
	char	*name;

	name = cmd->args[0];
	if (equal(name, "echo"))
		ft_echo(cmd->args);
	// else if (equal(name, "cd"))
	// {
	// 	// ft_cd(cmd->args);
	// }
	else if (equal(name, "pwd"))
		ft_pwd();
	// else if (equal(name, "export"))
	// {
	// 	// ft_export(cmd->args);
	// }
	else if (equal(name, "unset"))
		ft_unset(cmd->args, data->env);
	else if (equal(name, "env"))
		ft_env(data->env);
	else if (equal(name, "exit"))
		ft_exit(cmd->args);
	return (0);
}
