/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:41:36 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 16:13:39 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>

// Error Messages
# define ERR_CMD "command not found: "
# define ERR_INFILE "no such file or directory: "
# define ERR_ARGS "Invalid number of arguments."
# define ERR_PIPE "Pipe creation failed."
# define ERR_FORK "Fork creation failed."
# define ERR_HEREDOC "here_doc expected a LIMITER."

// Exit codes
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_CANNOT_EXECUTE 126

/*
 * here_doc           : 1 if here_doc is used, 0 otherwise
 * infile_fd          : file descriptor for the input file
 * outfile_fd         : file descriptor for the output file
 * cmd_count          : number of commands to execute
 * pids               : Array of process IDs for child processes
 * pipe_fd            : Current pipe file descriptors.
 * prev_pipe_read_end : Read end of the previous pipe
 * envp               : Environment variables.
 * cmd_paths          : Paths from the PATH environment variable.
 */
typedef struct s_pipex
{
	int		here_doc;
	int		infile_fd;
	int		outfile_fd;
	int		cmd_count;
	pid_t	*pids;
	int		pipe_fd[2];
	int		prev_pipe_read_end;
	char	**envp;
	char	**cmd_paths;
}			t_pipex;

// --- main.c ---
void		check_args(int argc, char **argv, t_pipex *data);

// --- init.c ---
void		init_pipex(t_pipex *data, int argc, char **argv, char **envp);
void		open_files(t_pipex *data, int argc, char **argv);

// --- execution.c ---
void		execute_pipeline(t_pipex *data, char **argv);
void		wait_for_children(t_pipex *data);

// --- heredoc.c ---
void		handle_heredoc(t_pipex *data, char *limiter);

// --- path.c ---
char		*find_command_path(char *cmd, char **paths);
void		get_cmd_paths(t_pipex *data);

// --- utils.c ---
void		msg_error_and_exit(char *msg, char *param, int code);
void		msg_perror_and_exit(char *msg, int code);

// --- cleanup.c ---
void		free_child_resources(t_pipex *data);
void		free_parent_resources(t_pipex *data);
void		close_fds(void);

#endif
