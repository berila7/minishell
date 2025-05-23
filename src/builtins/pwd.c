/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 20:38:21 by anachat           #+#    #+#             */
/*   Updated: 2025/05/23 16:15:44 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd && data->cwd_failed)
		return (1);
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
}
