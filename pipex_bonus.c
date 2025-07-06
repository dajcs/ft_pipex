/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:50:00 by anemet            #+#    #+#             */
/*   Updated: 2025/07/06 20:51:58 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// Waits for all child processes to complete.
// Returns the exit status of the last command in the pipeline
int	wait_for_children(int last_pid, int num_cmds)
{
	int	i;
	int	status;
	int	exit_status;

	i = 0;
	exit_status = 1;
	while (i < num_cmds)
	{
		if (wait(&status) == last_pid)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
		}
		i++;
	}
	return (exit_status);
}

// function called after fork() in the child process
// sets in_fd to STDIN and out_fd to STDOUT,
// closes redundant in_fd and out_fd
// executes the command
void	child_process(char *cmd, char **envp, int in_fd, int out_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
		perror_exit("dup2 failed for input", 1);
	if (dup2(out_fd, STDOUT_FILENO) == -1)
		perror_exit("dup2 failed for output", 1);
	close(in_fd);
	close(out_fd);
	execute_command(cmd, envp);
}

// the main loop for creating the pipeline.
// it forks a process for each command, connecting them with pipes
// - pipe_fd[0] - is the read end of the pipe (data comes out)
// - pipe_fd[1] - is the write end of the pipe (data goes in)
//  child proc:  'in_fd'-> cmd1 -> pipe_fd[1] (write) -> pipe
//..in_fd = pipe_fd[0] (read) -> cmd2 -> pipe_fd[1] (write) -> pipe
int	pipeline_loop(int argc, char **argv, char **envp, int in_fd)
{
	int		i;
	int		pipe_fd[2];
	int		cmd_start_idx;
	pid_t	pid;

	cmd_start_idx = 2;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		cmd_start_idx = 3;
	i = cmd_start_idx;
	while (i < argc - 2)
	{
		if (pipe(pipe_fd) == -1)
			perror_exit("pipe failed", 1);
		pid = fork();
		if (pid == -1)
			perror_exit("fork failed", 1);
		if (pid == 0)
			child_process(argv[i], envp, in_fd, pipe_fd[1]);
		close(in_fd);
		close(pipe_fd[1]);
		in_fd = pipe_fd[0];
		i++;
	}
	return (in_fd);
}

// sets in place `in_fd` and `out_fd` based on 'here_doc' argument
void	setup_io(int argc, char **argv, int *in_fd, int *out_fd)
{
	int	is_here_doc;

	is_here_doc = (ft_strncmp(argv[1], "here_doc", 9) == 0);
	if (is_here_doc)
	{
		if (argc < 6)
			exit_with_error("Usage: ./pipex here_doc LMT cmd1 ... file", 1);
		*in_fd = handle_here_doc(argv[2]);
		*out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*out_fd == -1)
			perror_exit(argv[argc - 1], 1);
	}
	else
	{
		*in_fd = open(argv[1], O_RDONLY);
		if (*in_fd == -1)
			perror_exit(argv[1], 1);
		*out_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*out_fd == -1)
			perror_exit(argv[argc - 1], 1);
	}
}

// setup_io sets `in_fd` and `out_fd` file descriptors
// pipeline_loop:
//  - sets up the pipeline, starts child cmd-s except the last one
//  - returns the in_fd to the last cmd
// fork:
//    - child_process handles last_pid
//    - parent process closes in_fd & out_fd and waits for finish
int	main(int argc, char *argv[], char **envp)
{
	int		in_fd;
	int		out_fd;
	int		num_cmds;
	pid_t	last_pid;

	if (argc < 5)
		exit_with_error("Usage: ./pipex file1 cmd1 cmd2 ... cmdX file2", 1);
	setup_io(argc, argv, &in_fd, &out_fd);
	in_fd = pipeline_loop(argc, argv, envp, in_fd);
	last_pid = fork();
	if (last_pid == -1)
		perror_exit("fork failed", 1);
	if (last_pid == 0)
		child_process(argv[argc - 2], envp, in_fd, out_fd);
	close(in_fd);
	close(out_fd);
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		num_cmds = argc - 4;
	else
		num_cmds = argc - 3;
	return (wait_for_children(last_pid, num_cmds));
}
