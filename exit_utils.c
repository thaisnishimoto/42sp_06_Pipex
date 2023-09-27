/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:45:50 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/27 02:06:52 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free_matrix(char **array, int size)
{
	int	i;

	i = 0;
	while(i <= size)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

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
	if (waitpid(process->id1, &process->wstatus1, 0) != process->id1)
		ft_handle_perror("waitpid1 error");
	if (waitpid(process->id2, &process->wstatus2, 0) == -1)
		ft_handle_perror("waitpid2 error");
	if (WIFEXITED(process->wstatus1))
	{
		ft_printf("exited 1\n");
	        process->exit_code = WEXITSTATUS(process->wstatus1);
	        if (process->exit_code == 126)
	                ft_printf("permission denied\n");
	        if (process->exit_code == 127)
	                ft_printf("command not found\n");
	}
	else if (WIFSIGNALED(process->wstatus1))
	        process->exit_code = WTERMSIG(process->wstatus1);
	if (WIFEXITED(process->wstatus2))
	{
		ft_printf("exited 2\n");
	        process->exit_code = WEXITSTATUS(process->wstatus2);
	        if (process->exit_code == 126)
	                ft_printf("permission denied\n");
	        if (process->exit_code == 127)
	                ft_printf("command not found\n");
	}
	else if (WIFSIGNALED(process->wstatus2))
	        process->exit_code = WTERMSIG(process->wstatus2);
}
