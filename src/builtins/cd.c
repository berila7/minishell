/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:22:49 by anachat           #+#    #+#             */
/*   Updated: 2025/06/11 17:47:32 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(t_data *data, char *path, char *oldpwd)
{
	char	*cwd;
	char	*pwd;
	char	*tmp;

	set_env(&data->gc, &data->env, "OLDPWD", oldpwd);
	free(oldpwd);
	pwd = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		tmp = get_env(data->env, "PWD");
		if (tmp[ft_strlen(tmp) - 1] == '/')
			pwd = gc_strjoin(&data->gc, tmp, path);
		else
			pwd = join_path(&data->gc, tmp, path);
		perror("cd: error retrieving current directory: getcwd");
	}
	else
		pwd = cwd;
	set_env(&data->gc, &data->env, "PWD", pwd);
	if (cwd)
		free(cwd);
}

int	ft_cd(char **args, t_data *data)
{
	char	*path;
	char	*oldpwd;
	int		ac;

	ac = count_args(args);
	if (ac > 2)
		return (print_err("%stoo many arguments\n", "cd: "), 1);
	else if (ac == 1)
	{
		path = get_env(data->env, "HOME");
		if (!path)
			return (print_err("%s: cd: HOME not set\n", "minishell"), 1);
	}
	else
		path = args[1];
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
		return (write(1, "cd: ", 5), perror(path), 1);
	else
		update_pwd(data, path, oldpwd);
	return (0);
}
