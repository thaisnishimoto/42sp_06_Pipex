/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:45:50 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/04 12:41:50 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

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
			ft_close_pipes(fd->pipe, pipex->pipe_count);
		if (stage == 3)
			free(pipex->pid);
		if (stage == 4)
			ft_free_matrix(pipex->cmd.args, pipex->cmd.args_count);
	}
	perror(error_msg);
	exit(EXIT_FAILURE);
}

void	ft_close_pipes(int **pipe, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipe[i][0]);
		close(pipe[i][1]);
		free(pipe[i]);
		i++;
	}
	free(pipe);
}

void	wait_finish_pipe(t_fd *fd, t_data *pipex)
{
	int	i;

	i = 0;
	ft_close_pipes(fd->pipe, pipex->pipe_count);
	while (i < pipex->cmd_count)
	{
		if (waitpid(pipex->pid[i], &pipex->wstatus, 0) == -1)
			ft_handle_error("waitpid error", pipex, fd, 1);
		i++;
	}
	if (WIFEXITED(pipex->wstatus))
		pipex->exit_code = WEXITSTATUS(pipex->wstatus);
	else if (WIFSIGNALED(pipex->wstatus))
		pipex->exit_code = WTERMSIG(pipex->wstatus);
	ft_free_matrix(pipex->path, pipex->path_count);
}
