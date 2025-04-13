/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:28:37 by anachat           #+#    #+#             */
/*   Updated: 2025/04/13 11:57:40 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_cmd(t_command *cmd, char **envp)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
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