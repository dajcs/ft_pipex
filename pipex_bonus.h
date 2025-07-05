/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:22:31 by anemet            #+#    #+#             */
/*   Updated: 2025/07/05 10:28:48 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// error_handler_bonus.c
void	exit_with_error(char *message, int exit_code);
void	perror_exit(char *message, int exit_code);

// pipex_utils_bonus.c
void	free_string_array(char **arr);
void	execute_command(char *cmd_arg, char **envp);

// here_doc_bonus.c
int		handle_here_doc(char *limiter);

// pipex_bonus.c
void	child_process(char *cmd, char **envp, int in_fd, int out_fd);
int		wait_for_children(int last_pid, int num_cmds);

#endif
