/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:07:57 by anachat           #+#    #+#             */
/*   Updated: 2025/05/17 18:48:27 by ayoub            ###   ########.fr       */
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

char	*find_cmd(char **paths, char *cmd, int *malloc_err)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		cmd_path = join_path(paths[i], cmd);
		if (!cmd_path)
			return (*malloc_err = 1, NULL);
		if (cmd_exists(cmd_path))
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

static char	*get_cmd_path(char *cmd, t_env *env, int *malloc_err)
{
	char	*env_path;
	char	*cmd_path;
	char	**paths;

	if (ft_strchr(cmd, '/'))
	{
		if (!cmd_exists(cmd))
			return (NULL);
		cmd_path = ft_strdup(cmd);
		if (!cmd_path)
			*malloc_err = 1;
		return (cmd_path);
	}
	env_path = get_env_path(env);
	if (!env_path)
		return (printf("Cannot find PATH env\n"), NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (*malloc_err = 1, NULL);
	cmd_path = find_cmd(paths, cmd, malloc_err);
	free_arr(paths);
	return (cmd_path);
}

int	set_cmd_path(t_cmd *cmds, t_env *env)
{
	t_cmd	*cmd;
	int		malloc_err;

	malloc_err = 0;
	cmd = cmds;
	while (cmd)
	{
		if (cmd->args && cmd->args[0] && cmd->args[0][0])
		{
			cmd->path = get_cmd_path(cmd->args[0], env, &malloc_err);
			if (malloc_err)
				return (print_err("My Allocation Error\n", NULL), 1);
		}
		cmd = cmd->next;
	}
	return (0);
}
