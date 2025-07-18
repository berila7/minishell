/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:07:57 by anachat           #+#    #+#             */
/*   Updated: 2025/06/20 16:12:17 by anachat          ###   ########.fr       */
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

char	*in_cur_dir(t_gcnode **gc, char *cmd)
{
	char	*path;

	if (ft_strchr(cmd, '/'))
		return (NULL);
	path = gc_strjoin(gc, "./", cmd);
	if (cmd_exists(path))
		return (path);
	return (NULL);
}

static char	*get_cmd_path(t_gcnode **gc, char *cmd, t_env *env)
{
	char	*env_path;
	char	*cmd_path;
	char	**paths;

	if (ft_strchr(cmd, '/'))
	{
		cmd_path = gc_strdup(gc, cmd);
		return (cmd_path);
	}
	env_path = get_env_path(env);
	if (!env_path || env_path[0] == '\0' || !cmd[0])
		return (gc_strdup(gc, cmd));
	paths = gc_split_char(gc, env_path, ':');
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
		{
			if (equal(".", cmd->args[0]))
				cmd->path = NULL;
			else if (equal("..", cmd->args[0]))
				cmd->path = gc_strdup(gc, "..");
			else if (is_builtin(cmd))
				cmd->path = gc_strdup(gc, "builtin");
			else
				cmd->path = get_cmd_path(gc, cmd->args[0], env);
		}
		cmd = cmd->next;
	}
}
