/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:38:21 by anachat           #+#    #+#             */
/*   Updated: 2025/05/03 12:02:07 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_data *data)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("my cwd: %s\n", cwd);
		free(cwd);
	}
	else
		printf("this is env: %s\n", get_env(data->env, "PWD"));
}
