/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberila <mberila@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:28:37 by anachat           #+#    #+#             */
/*   Updated: 2025/04/13 14:06:22 by mberila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_cmd(t_cmd *cmd, char **envp)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		printf("failed to fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		// child process
		if (execve(cmd->path, cmd->args, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		// parent process
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
}