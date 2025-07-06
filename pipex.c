/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:07:50 by anemet            #+#    #+#             */
/*   Updated: 2025/07/06 23:31:09 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// handles the first child process (cmd1):
// - pipe_fd[1] - is the write end of the pipe (data goes in)
//     - opens the infile (file1)
//     - redirects (dup2) stdin to infile (get input data from file1)
//     - redirects (dup2) stdout to the write-end of the pipe
void	child_one_process(char **argv, int *pipe_fd, char **envp)
{
	int	infile_fd;

	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
		perror_exit(argv[1], 1);
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		perror_exit("dup2 failed for infile", 1);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		perror_exit("dup2 failed for pipe write-end", 1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(infile_fd);
	execute_command(argv[2], envp);
}

// handles the second child process (cmd2):
// - pipe_fd[0] - is the read end of the pipe (data comes out)
//     - opens the outfile (file2)
//     - redirects (dup2) stdin to the read-end of the pipe
//     - redirects (dup2) stdout to the outfile (file2)
void	child_two_process(char **argv, int *pipe_fd, char **envp)
{
	int	outfile_fd;

	outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == -1)
		perror_exit(argv[4], 1);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		perror_exit("dup2 failed for pipe read-end", 1);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		perror_exit("dup2 failed for outfile", 1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(outfile_fd);
	execute_command(argv[3], envp);
}

// waits for child processes to finish and returns exit status of the last one
// Waits for all child processes to complete.
// Returns the exit status of the last command in the pipeline
// wait() suspends execution of current (parent) process until a child finishes
// returns the PID of the child
// sets the status variable with a bitmask
// WIFEXITED - macro to ask if terminate normally by calling exit()
// WEXITSTATUS - macro to get exit code e.g. 0 success, 127 command not found
// WIFSIGNALED - macro to ask if terminated by signal (e.g. kill <pid>)
// WTERMSIG - to get the killer signal id
// standard way: wait() -> WIFEXITED -> WEXITSTATUS
// only the last_pid exit status matters, because that is replicating the shell
int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status;
	int	exit_status;

	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else
		exit_status = 1;
	return (exit_status);
}

// main function: argument validation, pipe creation, forking and waiting
// pipe(pipe_fd) - try to open a pipe, if ok put fd-s in `pipe_fd` array
// fork() - try to fork, return pid (process id)
//     - if pid == -1 -> gempa
//     - if pid == 0 we're in the child process -> go and execute child stuff
//     - if pid > 0 we're still in the parent process
//         -- close pipe fd-s in the parent
//         -- wait for children to finish their jobs
int	main(int argc, char *argv[], char **envp)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
		exit_with_error("Usage: ./pipex file1 cmd1 cmd2 file2", 1);
	if (pipe(pipe_fd) == -1)
		perror_exit("pipe failed", 1);
	pid1 = fork();
	if (pid1 == -1)
		perror_exit("fork1 failed", 1);
	if (pid1 == 0)
		child_one_process(argv, pipe_fd, envp);
	pid2 = fork();
	if (pid2 == -1)
		perror_exit("fork2 failed", 1);
	if (pid2 == 0)
		child_two_process(argv, pipe_fd, envp);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_for_children(pid1, pid2));
}
