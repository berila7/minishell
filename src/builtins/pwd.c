/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:38:21 by anachat           #+#    #+#             */
/*   Updated: 2025/06/13 16:08:11 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd && data->cwd_failed)
	{
		print_err("pwd: error retrieving current directory: getcwd: %s\n",
			"cannot access parent directories: No such file or directory");
		return (1);
	}
	if (!cwd)
	{
		cwd = get_env(data->env, "PWD");
		printf("%s\n", cwd);
	}
	else
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	return (0);
}
