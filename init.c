/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:36:17 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 16:52:38 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// opens the input and output files based on whether `here_doc` is used.
// for `here_doc`, output is appended. Otherwise it is truncated.
void	open_files(t_pipex *data, int argc, char **argv)
{
	if (data->here_doc)
	{
		handle_heredoc(data, argv[2]);
		data->outfile_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND,
				0644);
	}
	else
	{
		data->infile_fd = open(argv[1], O_RDONLY);
		if (data->infile_fd < 0)
			msg_error_and_exit(ERR_INFILE, argv[1], EXIT_FAILURE);
		data->outfile_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (data->outfile_fd < 0)
		msg_perror_and_exit("outfile", EXIT_FAILURE);
}

// initializes the t_pipex structure with necessary values
void init_pipex(t_pipex *data, int argc, char **argv, char **envp)
{
	data->envp = envp;
	open_files(data, argc, argv);
	get_cmd_paths(data);
	if (data->here_doc)
		data->cmd_count = argc - 4;
	else
		data->cmd_count = argc - 3;
	data->pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!data->pids)
		msg_perror_and_exit("malloc pids", EXIT_FAILURE);
	data->prev_pipe_read_end = data->infile_fd;
}
