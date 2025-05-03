/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:22:49 by anachat           #+#    #+#             */
/*   Updated: 2025/05/03 12:55:15 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_cd_error(const char *path)
{
	if (errno == ENOENT)
		printf("cd: %s: No such file or directory\n", path);
	else if (errno == ENOTDIR)
		printf("cd: %s: Not a directory\n", path);
	else if (errno == EACCES)
		printf("cd: %s: Permission denied\n", path);
	else
		printf("cd: %s: %s\n", path, strerror(errno));
}

void	ft_cd(char **args, t_data *data)
{
	char	*path;
	char	*cwd;
	int		ac;

	ac = count_args(args);
	if (ac == 1)
		path = get_env(data->env, "HOME");
	else
		path = args[1];
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		if (chdir(path) == -1)
			ft_cd_error(path);
		else if (cwd)
		{
			set_env(&(data->env), "OLDPWD", cwd);
			free(cwd);
			cwd = getcwd(NULL, 0);
			set_env(&(data->env), "PWD", cwd);
		}
	}
	else
	{
		if (equal(path, ".") || equal(path, ".."))
		{	
			cwd = ft_strjoin(get_env(data->env, "PWD"), "/");
			cwd = ft_strjoin(cwd, path);
		}
		else
			perror("getcwd");
		set_env(&(data->env), "PWD", cwd);
		free(cwd);
	}
}
