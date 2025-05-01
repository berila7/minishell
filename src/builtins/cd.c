/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:22:49 by anachat           #+#    #+#             */
/*   Updated: 2025/05/01 18:24:30 by anachat          ###   ########.fr       */
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

	cwd = getcwd(NULL, 0);
	ac = count_args(args);
	if (ac == 1)
		path = get_env(data->env, "HOME");
	else if (equal(args[1], "--"))
		path = get_env(data->env, "OLDPWD");
	else if (equal(args[1], "-"))
	{
		path = get_env(data->env, "OLDPWD");
		printf("%s\n", path);
	}
	else
		path = args[1];
	if (chdir(path) == -1)
		ft_cd_error(path);
	else
		set_env(&(data->env), "OLDPWD", cwd);
	free(cwd);
}
