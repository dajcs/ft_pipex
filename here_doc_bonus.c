/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:37:21 by anemet            #+#    #+#             */
/*   Updated: 2025/07/05 11:22:10 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// reads from stdin line by line until a line matching 'limiter` is found.
// the content is written to a pipe, and the read-end of the pipe is returned
int handle_here_doc(char *limiter)
{
	int pipe_fd[2];
	char *line;
	char *limiter_nl;

	if (pipe(pipe_fd) == -1)
		perror_exit("pipe failed", 1);
	limiter_nl = ft_strjoin(limiter, "\n");
	if (!limiter_nl)
		perror_exit("malloc failed", 1);
	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	while (line)
	{
		if (ft_strncmp(line, limiter_nl, ft_strlen(limiter_nl) + 1) == 0)
		{
			free(line);
			break;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
	}
	free(limiter_nl);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
