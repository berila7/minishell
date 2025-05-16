/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:22:49 by anachat           #+#    #+#             */
/*   Updated: 2025/05/16 16:11:10 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exit_program(t_data *data)
{
	free_data(data);
	exit(1);
}

int	update_pwd(t_data *data, char *path)
{
	char	*cwd;
	char	*pwd;
	char	*tmp;

	pwd = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		tmp = get_env(data->env, "PWD");
		if (tmp[ft_strlen(tmp) - 1] == '/')
			pwd = ft_strjoin(tmp, path);
		else
			pwd = join_path(tmp, path);
		if (!pwd)
			exit_program(data);
		perror("getcwd");
	}
	else
		pwd = cwd;
	set_env(&(data->env), "PWD", pwd);
	if (cwd)
		free(cwd);
	return (0);
}

void	ft_cd(char **args, t_data *data)
{
	char	*path;
	int		ac;

	ac = count_args(args);
	if (ac > 2)
	{
		print_err("cd: too many arguments\n", NULL);
		return ;
	}
	else if (ac == 1)
		path = get_env(data->env, "HOME");
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		write(1, "cd: ", 5);
		perror(path);
	}
	else
		update_pwd(data, path);
}
