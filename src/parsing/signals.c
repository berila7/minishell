/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:28:43 by berila            #+#    #+#             */
/*   Updated: 2025/05/22 11:25:04 by berila           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

void	signal_handler_interactive(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
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
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

void	signal_handler_exec(int signum)
{
	if (signum == SIGINT)
	{
		exit_status(130, 1);
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (signum == SIGQUIT)
	{
		exit_status(131, 1);
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
