/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:45:50 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/20 19:22:54 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_handle_perror(char *perror_msg)
{
	perror(perror_msg);
	exit(EXIT_FAILURE);
}

void	ft_handle_error(char *error_msg)
{
	ft_putstr_fd(error_msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	ft_close_pipe(t_fd *fd)
{
	close(fd->pipe[0]);
	close(fd->pipe[1]);
}

void	wait_finish_pipe(t_fd *fd, t_fork *process)
{
	ft_close_pipe(fd);
	waitpid(process->id1, &process->wstatus1, 0);
	waitpid(process->id2, &process->wstatus2, 0);
	if (WIFEXITED(process->wstatus2))
	{
	        process->exit_code = WEXITSTATUS(process->wstatus2);
	        if (process->exit_code == 127)
	                ft_printf("command not found\n");
	                //ft_printf("%s %s: command not found\n", argv[1], argv[3]);
	}
}
