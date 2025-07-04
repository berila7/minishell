/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 11:33:22 by anachat           #+#    #+#             */
/*   Updated: 2025/06/18 15:04:36 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_interactive_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = &signal_handler_interactive;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		perror("sigaction SIGINT interactive");
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		perror("sigaction SIGQUIT interactive");
}

void	setup_parent_waiting_signals(void)
{
	struct sigaction	sa_ign;

	sa_ign.sa_handler = SIG_IGN;
	sigemptyset(&sa_ign.sa_mask);
	sa_ign.sa_flags = 0;
	if (sigaction(SIGINT, &sa_ign, NULL) == -1)
		perror("sigaction SIGINT parent waiting");
	if (sigaction(SIGQUIT, &sa_ign, NULL) == -1)
		perror("sigaction SIGQUIT parent waiting");
}

void	setup_child_default_signals(void)
{
	struct sigaction	sa_dfl;

	sa_dfl.sa_handler = SIG_DFL;
	sigemptyset(&sa_dfl.sa_mask);
	sa_dfl.sa_flags = 0;
	if (sigaction(SIGINT, &sa_dfl, NULL) == -1)
		perror("sigaction SIGINT child default");
	if (sigaction(SIGQUIT, &sa_dfl, NULL) == -1)
		perror("sigaction SIGQUIT child default");
}

void	reset_to_system_default_signals(void)
{
	struct sigaction	sa_dfl;

	sa_dfl.sa_handler = SIG_DFL;
	sigemptyset(&sa_dfl.sa_mask);
	sa_dfl.sa_flags = 0;
	sigaction(SIGINT, &sa_dfl, NULL);
	sigaction(SIGQUIT, &sa_dfl, NULL);
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa_int;

	g_sigint_received = 0;
	sa_int.sa_handler = &signal_handler_heredoc;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}
