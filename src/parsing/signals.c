/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: berila <berila@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:28:43 by berila            #+#    #+#             */
/*   Updated: 2025/04/30 18:09:50 by berila           ###   ########.fr       */
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
        // Set the flag
        g_sigint_received = 1;
        
        // Write newline for visual clarity
        write(STDOUT_FILENO, "\n", 1);
        
        // This is the key part - we need to force readline to return
        rl_done = 1;        // This tells readline to return immediately
        rl_event_hook = 0;  // Clear any event hooks
    }
}

void	signal_handler_exec(int signum)
{
	if (signum == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (signum == SIGQUIT)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = &signal_handler_interactive;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	signal(SIGQUIT, SIG_IGN);
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

void reset_readline_signal_handlers(void)
{
    rl_catch_signals = 1;  // Let readline handle signals in its own way
    rl_set_signals();     // Reset readline's signal handlers
}