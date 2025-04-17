/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multiple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anachat <anachat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:40:48 by anachat           #+#    #+#             */
/*   Updated: 2025/04/16 18:19:11 by anachat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_pipeline {
    int prev_pipe[2];  // Pipe from previous command
    int curr_pipe[2];  // Pipe for current command
} t_pipeline;

// int child1(t_cmd *cmd, int i, t_data *data, t_pipeline *pipes)
// {
//     pid_t id;

//     if (cmd->next) {  // Only create new pipe if there's a next command
//         if (pipe(pipes->curr_pipe) < 0)
//             return (perror("pipe failed"), 1);
//     }

//     id = fork();
//     if (id < 0)
//         return (perror("fork failed"), 1);
//     if (id == 0)
//     {
//         if (handle_redirections(cmd))
//             return (1);
        
//         // First command
//         if (i == 0) {
//             if (!cmd->output_file && cmd->next)
//                 ft_dup2(pipes->curr_pipe[1], STDOUT_FILENO);
//         }
//         // Last command
//         else if (!cmd->next) {
//             if (!cmd->input_file)
//                 ft_dup2(pipes->prev_pipe[0], STDIN_FILENO);
//         }
//         // Middle commands
//         else {
//             if (!cmd->input_file)
//                 ft_dup2(pipes->prev_pipe[0], STDIN_FILENO);
//             if (!cmd->output_file)
//                 ft_dup2(pipes->curr_pipe[1], STDOUT_FILENO);
//         }

//         // Close all pipe fds in child
//         if (i > 0) {
//             close(pipes->prev_pipe[0]);
//             close(pipes->prev_pipe[1]);
//         }
//         if (cmd->next) {
//             close(pipes->curr_pipe[0]);
//             close(pipes->curr_pipe[1]);
//         }

//         if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
//         {
//             perror("execve failed");
//             exit(1);
//         }
//         return (0);
//     }

//     // Parent process: Close previous pipe and update for next iteration
//     if (i > 0) {
//         close(pipes->prev_pipe[0]);
//         close(pipes->prev_pipe[1]);
//     }
//     if (cmd->next) {
//         pipes->prev_pipe[0] = pipes->curr_pipe[0];
//         pipes->prev_pipe[1] = pipes->curr_pipe[1];
//     }

//     return (id);
// }

// int exec_multiple_cmd(t_data *data)
// {
//     t_pipeline pipes = {{-1, -1}, {-1, -1}};
//     pid_t last_pid;
//     t_cmd *cmd;
//     int i;

//     cmd = data->cmds;
//     if (!cmd->path)
//         return (printf("%s: command not found\n", cmd->args[0]), 1);
    
//     i = 0;
//     while (cmd)
//     {
//         last_pid = child1(cmd, i, data, &pipes);
//         cmd = cmd->next;
//         i++;
//     }

//     // Wait for all child processes
//     while (wait(NULL) != -1)
//         ;
//     return (0);
// }




int child1(t_cmd *cmd, int i, t_data *data, int *pid)
{
	pid_t id;

	if (cmd->next)
	{
		if (pipe(data->curr_pipe) < 0)
			return (perror("pipe failed"), 1);
	}
	id = fork();
	if (id < 0)
		return (perror("fork failed"), 1);
	if (id == 0)
	{
		*pid = id;
		if (handle_redirections(cmd))
			return (1);
		// if first cmd and outfile is null then use pipe as output
		if (i == 0 && !cmd->output_file)
		{
			ft_dup2(data->curr_pipe[1], STDOUT_FILENO);
			close(data->curr_pipe[0]); // close unused pipe
			close(data->prev_pipe[0]); // close unused pipe
			close(data->prev_pipe[1]); // close unused pipe
		}
		else if (!cmd->next) // last cmd
		{
			if (!cmd->input_file)
				ft_dup2(data->prev_pipe[0], STDIN_FILENO);
			close(data->prev_pipe[1]); // close unused pipe
			close(data->curr_pipe[0]); // close unused pipe
			close(data->curr_pipe[1]); // close unused pipe
		}
		else
		{
			if (!cmd->input_file)
				ft_dup2(data->prev_pipe[0], STDIN_FILENO);
			if (!cmd->output_file)
				ft_dup2(data->curr_pipe[1], STDOUT_FILENO);
			close(data->prev_pipe[1]); // close unused pipe
			close(data->curr_pipe[0]); // close unused pipe
		}
		if (execve(cmd->path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve failed");
			exit(1);
		}
		return (0);
	}
	else
	{
		if (i > 0)
		{
			close(data->prev_pipe[0]); // close unused pipe
			close(data->prev_pipe[1]); // close unused pipe
		}
		if (cmd->next)
		{
			data->prev_pipe[0] = data->curr_pipe[0]; // forward pipes
			data->prev_pipe[1] = data->curr_pipe[1]; // forward pipes
		}
		return (0);
	}
}

int	exec_multiple_cmd(t_data *data)
{
	pid_t	last_pid;
	t_cmd	*cmd;
	int		i;

	last_pid = 0;
	cmd = data->cmds;
	if (!cmd->path)
		return (printf("%s: command not found\n", cmd->args[0]), 1);
	i = 0;
	while (cmd)
	{
		child1(cmd, i, data, &last_pid);
		cmd = cmd->next;
		i++;
	}
	while (wait(NULL) != -1)
		;
	return (0);
	// return (ft_wait(last_pid, 0));
}
