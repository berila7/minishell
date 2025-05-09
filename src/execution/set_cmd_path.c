/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:07:57 by anachat           #+#    #+#             */
/*   Updated: 2025/05/09 09:54:23 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_path(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*find_cmd(char **paths, char *cmd)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		cmd_path = join_path(paths[i], cmd);
		if (!cmd_path)
			return (printf("allocation error\n"), NULL);
		if (cmd_exists(cmd_path))
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, t_env *env)
{
	char	*env_path;
	char	*cmd_path;
	char	**paths;

	if (ft_strchr(cmd, '/'))
	{
		if (!cmd_exists(cmd))
			return (NULL);
		return (ft_strdup(cmd));
	}
	env_path = get_env_path(env);
	if (!env_path)
		return (printf("Cannot find PATH env\n"), NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (printf("allocation error\n"), NULL);
	cmd_path = find_cmd(paths, cmd);
	free_arr(paths);
	return (cmd_path);
}

void	set_cmd_path(t_cmd *cmds, t_env *env)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (cmd->args && cmd->args[0] && cmd->args[0][0])
			cmd->path = get_cmd_path(cmd->args[0], env);
		cmd = cmd->next;
	}
}
