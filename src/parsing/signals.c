/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:28:43 by berila            #+#    #+#             */
/*   Updated: 2025/05/05 16:19:22 by mberila          ###   ########.fr       */
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

void signal_handler_heredoc(int signum)
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
		write(STDOUT_FILENO, "\n", 1);
	else if (signum == SIGQUIT)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}

void setup_interactive_signals(void)
{
    struct sigaction sa_int;

    sa_int.sa_handler = &signal_handler_interactive;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);

    signal(SIGQUIT, SIG_IGN);
	rl_on_new_line();
}


void	setup_heredoc_signals(void)
{
	g_sigint_received = 0;
	
	struct sigaction	sa_int;

	sa_int.sa_handler = &signal_handler_heredoc;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	signal(SIGQUIT, SIG_IGN);
}

void	setup_exec_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = &signal_handler_exec;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}