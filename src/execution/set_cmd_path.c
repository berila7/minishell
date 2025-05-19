/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:07:57 by anachat           #+#    #+#             */
/*   Updated: 2025/05/19 10:29:27 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_path(t_env *env)
{
	while (env)
	{
		if (env->value && equal(env->key, "PATH"))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*find_cmd(t_gcnode **gc, char **paths, char *cmd)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		cmd_path = join_path(gc, paths[i], cmd);
		if (cmd_exists(cmd_path))
			return (cmd_path);
		gc_free(gc, cmd_path);
		i++;
	}
	return (NULL);
}

static char	*get_cmd_path(t_gcnode **gc, char *cmd, t_env *env)
{
	char	*env_path;
	char	*cmd_path;
	char	**paths;

	if (ft_strchr(cmd, '/'))
	{
		if (!cmd_exists(cmd))
			return (NULL);
		cmd_path = gc_strdup(gc, cmd);
		return (cmd_path);
	}
	env_path = get_env_path(env);
	if (!env_path)
		return (printf("Cannot find PATH env\n"), NULL);
	paths = gc_split(gc, env_path, ':');
	cmd_path = find_cmd(gc, paths, cmd);
	free_arr(gc, paths);
	return (cmd_path);
}

void	set_cmd_path(t_gcnode **gc, t_cmd *cmds, t_env *env)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (cmd->args && cmd->args[0] && cmd->args[0][0])
			cmd->path = get_cmd_path(gc, cmd->args[0], env);
		cmd = cmd->next;
	}
}
