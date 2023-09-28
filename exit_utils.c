/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:45:50 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/28 17:48:44 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free_matrix(char **array, int size)
{
	int	i;

	i = 0;
	while (i <= size)
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

void	wait_finish_pipe(t_fd *fd, t_fork *process, t_data *pipex)
{
	ft_close_pipe(fd);
	if (waitpid(process->id1, &process->wstatus1, 0) == -1)
		ft_handle_perror("waitpid1 error");
	if (waitpid(process->id2, &process->wstatus2, 0) == -1)
		ft_handle_perror("waitpid2 error");
	if (WIFEXITED(process->wstatus2))
		process->exit_code = WEXITSTATUS(process->wstatus2);
	else if (WIFSIGNALED(process->wstatus2))
		process->exit_code = WTERMSIG(process->wstatus2);
	ft_free_matrix(pipex->path, pipex->path_count);
}
