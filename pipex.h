/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:00:26 by anemet            #+#    #+#             */
/*   Updated: 2025/07/04 14:01:54 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// error_handler.c
void	exit_with_error(char *message, int exit_code);
void	perror_exit(char *message, int exit_code);

// pipex_utils.c
void	free_string_array(char **arr);
char	*find_path_in_env(char **envp);
char	*get_command_path(char *command, char **envp);
void	execute_command(char *cmd_arg, char **envp);

// pipex.c
void	child_one_process(char **argv, int *pipe_fd, char **envp);
void	child_two_process(char **argv, int *pipe_fd, char **envp);

#endif
