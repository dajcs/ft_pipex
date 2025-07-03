/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:17:22 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 16:32:03 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// check nr of arguments
// min 5 for standard case and min 6 for here_doc case
void	check_args(int argc, char **argv, t_pipex *data)
{
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
			msg_error_and_exit(ERR_ARGS, NULL, EXIT_FAILURE);
		data->here_doc = 1;
	}
	else
	{
		if (argc < 5)
			msg_error_and_exit(ERR_ARGS, NULL, EXIT_FAILURE);
		data->here_doc = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	data;

	if (argc < 5)
		msg_error_and_exit(ERR_ARGS, NULL, EXIT_FAILURE);
	ft_bzero(&data, sizeof(t_pipex));
	check_args(argc, argv, &data);
	init_pipex(&data, argc, argv, envp);
	execute_pipeline(&data, argv);
	wait_for_children(&data);
	free_parent_resources(&data);
	return (EXIT_SUCCESS);
}
