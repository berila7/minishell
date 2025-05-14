/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:17:53 by anachat           #+#    #+#             */
/*   Updated: 2025/05/14 17:19:09 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close2(int *fds)
{
	close(fds[0]);
	close(fds[1]);
}

void	close_pipes(t_data *data)
{
	close2(data->pipe);
}

void	dup2_og(t_data *data)
{
	ft_dup2(data->og_fd[0], STDIN_FILENO);
	ft_dup2(data->og_fd[1], STDOUT_FILENO);
}

void	dup_og(t_data *data)
{
	data->og_fd[0] = dup(STDIN_FILENO);
	data->og_fd[1] = dup(STDOUT_FILENO);
}

int	ft_dup2(int oldfd, int newfd)
{
	int	res;

	res = dup2(oldfd, newfd);
	close(oldfd);
	return (res);
}
