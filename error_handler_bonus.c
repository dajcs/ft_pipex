/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:31:00 by anemet            #+#    #+#             */
/*   Updated: 2025/07/05 10:36:51 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// the rest is identical to error_handler.c

// uses `perror` to print the system error message corresponding to errno,
// preceded by a custom message, then exits with the given code
// Useful for system call failures
void	perror_exit(char *message, int exit_code)
{
	perror(message);
	exit(exit_code);
}

// print a custom error message to stderr and exits the program with given code
void	exit_with_error(char *message, int exit_code)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	exit(exit_code);
}
