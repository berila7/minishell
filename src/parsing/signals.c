/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:28:43 by berila            #+#    #+#             */
/*   Updated: 2025/06/13 16:04:07 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sigint_received = 0;

void	signal_handler_interactive(int signum)
{
	if (signum == SIGINT)
	{
		exit_status(130, 1);
		write(STDOUT_FILENO, "\n", 2);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint_received = 1;
		close(STDIN_FILENO);
	}
}
