/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:32:40 by anemet            #+#    #+#             */
/*   Updated: 2025/07/06 22:56:47 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// the rest is identical to pipex_utils.c

// frees a dynamically allocated array of strings (e.g., from ft_split)
void	free_string_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

// searches the environment variables for the "PATH" string
char	*find_path_in_env(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

// scan all paths + cmd is accessible or not
// return successful result or NULL if no one is successful
char	*scan_all_paths(char **all_paths, char *command)
{
	int		i;
	char	*temp;
	char	*cmd_path;

	i = 0;
	while (all_paths && all_paths[i])
	{
		temp = ft_strjoin(all_paths[i], "/");
		cmd_path = ft_strjoin(temp, command);
		free(temp);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_string_array(all_paths);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_string_array(all_paths);
	return (NULL);
}

// finds the full path of a command by searching directories in the PATH
// returns a dynamically allocated string with the full path or NULL
char	*get_command_path(char *command, char **envp)
{
	char	**all_paths;
	char	*path_env;

	if (access(command, F_OK | X_OK) == 0)
		return (ft_strdup(command));
	path_env = find_path_in_env(envp);
	if (!path_env)
		return (NULL);
	all_paths = ft_split(path_env, ':');
	return (scan_all_paths(all_paths, command));
}

// parses the command argument, finds the executable path and runs `execve`
// returns only when there is an execution error
void	execute_command(char *cmd_arg, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(cmd_arg, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		free_string_array(cmd_args);
		exit_with_error("command not found: ", 127);
	}
	cmd_path = get_command_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd_args[0], 2);
		free_string_array(cmd_args);
		exit(127);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		free(cmd_path);
		free_string_array(cmd_args);
		perror_exit("pipex: execve failed", 126);
	}
}
