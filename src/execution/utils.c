/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:03:22 by anachat           #+#    #+#             */
/*   Updated: 2025/04/24 15:41:39 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_dup2(int oldfd, int newfd)
{
	int	res;

	res = dup2(oldfd, newfd);
	close(oldfd);
	return (res);
}
int is_exec(char *path)
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

char	*join_path(char *path, char *cmd)
{
	char	*cmd_path;
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	cmd_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!cmd_path)
		return (NULL);
	return (cmd_path);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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
