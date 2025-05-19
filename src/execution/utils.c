/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:03:22 by anachat           #+#    #+#             */
/*   Updated: 2025/05/19 10:30:22 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_exec(char *path)
{
	if (access(path, X_OK) == 0)
		return (1);
	return (0);
}

int	cmd_exists(char *path)
{
	if (access(path, F_OK) == 0)
		return (1);
	return (0);
}

char	*join_path(t_gcnode **gc, char *path, char *cmd)
{
	char	*cmd_path;
	char	*tmp;

	tmp = gc_strjoin(gc, path, "/");
	if (!tmp)
		return (NULL);
	cmd_path = gc_strjoin(gc, tmp, cmd);
	gc_free(gc, tmp);
	if (!cmd_path)
		return (NULL);
	return (cmd_path);
}

void	free_arr(t_gcnode **gc, char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		gc_free(gc, arr[i]);
		i++;
	}
	gc_free(gc, arr);
}

int	count_cmd(t_cmd *cmds)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = cmds;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}
