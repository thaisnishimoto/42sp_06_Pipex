/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:45:50 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/10 15:20:25 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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

void	ft_handle_error(char *error_msg, t_data *pipex, t_fd *fd, int stage)
{
	if (stage >= 1)
	{
		ft_free_matrix(pipex->path, pipex->path_count);
		if (stage >= 2)
			ft_close_pipe(fd);
		if (stage == 3)
			ft_free_matrix(pipex->cmd.args, pipex->cmd.args_count);
	}
	perror(error_msg);
	exit(EXIT_FAILURE);
}

void	ft_close_pipe(t_fd *fd)
{
	close(fd->pipe[0]);
	close(fd->pipe[1]);
}

void	ft_close_stdfileno(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	wait_finish_pipe(t_fd *fd, t_fork *process, t_data *pipex)
{
	ft_close_pipe(fd);
	if (waitpid(process->id1, &process->wstatus1, 0) == -1)
		ft_handle_error("waitpid1 error", pipex, fd, 1);
	if (waitpid(process->id2, &process->wstatus2, 0) == -1)
		ft_handle_error("waitpid2 error", pipex, fd, 1);
	if (WIFEXITED(process->wstatus2))
		process->exit_code = WEXITSTATUS(process->wstatus2);
	else if (WIFSIGNALED(process->wstatus2))
		process->exit_code = WTERMSIG(process->wstatus2);
	ft_free_matrix(pipex->path, pipex->path_count);
	ft_close_stdfileno();
}
