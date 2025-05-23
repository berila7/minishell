/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:02:05 by anachat           #+#    #+#             */
/*   Updated: 2025/05/23 11:18:43 by ayoub            ###   ########.fr       */
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

int	open_outfile(char *file, int mode)
{
	int	fd;

	if (mode == 1)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (fd);
}

int	open_infile(char *path, t_cmd *cmd)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (perror(path), 1);
	if (count_args(cmd->args) > 0)
		ft_dup2(fd, STDIN_FILENO);
	return (0);
}

int	is_valid_env_key(char *str)
{
	size_t	i;

	if (!str || (str[0] != '_' && !ft_isalpha(str[0])))
		return (0);
	i = -1;
	while (str[++i])
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
			return (0);
	}
	return (1);
}
