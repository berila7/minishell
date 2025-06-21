/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:01:55 by mberila           #+#    #+#             */
/*   Updated: 2025/06/21 09:50:36 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_status(int status, int update)
{
	static int	exit_status;

	if (update)
	{
		exit_status = status;
		// print_err("===> Exit Status [%d]\n", &exit_status);
	}
	return (exit_status);
}
