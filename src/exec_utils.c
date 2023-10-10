/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/10 18:21:17 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	open_infile(t_fd *fd, char *argv[], t_data *pipex)
{
	fd->infile = open(argv[1], O_RDONLY);
	if (fd->infile < 0)
	{
		ft_printf("%s: ", argv[1]);
		ft_handle_error("", pipex, fd, 4);
	}
}

void	open_outfile(t_fd *fd, char *argv[], t_data *pipex)
{
	char	*outfile_path;

	outfile_path = argv[pipex->argc -1];
	fd->outfile = open(outfile_path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (fd->outfile < 0)
	{
		ft_printf("%s: ", argv[pipex->argc - 1]);
		ft_handle_error("", pipex, fd, 4);
	}
}

void	test_cmd_permission(char **path, char **cmd_args, t_data *pipex)
{
	int	i;

	i = 0;
	while (path[i])
	{
		pipex->exit_code = 0;
		if (cmd_args[0][0] == '/')
			pipex->pathname = ft_str_rm_dup(cmd_args[0], '/');
		else
			pipex->pathname = ft_strjoin(path[i], cmd_args[0]);
		if (access(pipex->pathname, F_OK) == -1)
		{
			pipex->exit_code = 127;
			free(pipex->pathname);
			i++;
		}
		else if (access(pipex->pathname, X_OK) == -1)
		{
			pipex->exit_code = 126;
			free(pipex->pathname);
			break ;
		}
		else
			return ;
	}
}

int	redirect_stdin_stdout(int i, t_fd *fd, t_data *pipex)
{
	if (i == 0)
	{
		if (dup2(fd->infile, STDIN_FILENO) == -1)
			return (-1);
		close(fd->infile);
	}
	else
	{
		if (dup2(fd->pipe[i - 1][0], STDIN_FILENO) == -1)
			return (-1);
	}
	if (i == pipex->cmd_count - 1)
	{
		if (dup2(fd->outfile, STDOUT_FILENO) == -1)
			return (-1);
		close(fd->outfile);
	}
	else
	{
		if (dup2(fd->pipe[i][1], STDOUT_FILENO) == -1)
			return (-1);
	}
	ft_close_pipes(fd->pipe, pipex->pipe_count);
	return (0);
}

void	exec_cmd(int i, t_fd fd, char *argv[], t_data *pipex)
{
	if (i == 0)
		open_infile(&fd, argv, pipex);
	if (i == pipex->cmd_count - 1)
		open_outfile(&fd, argv, pipex);
	if (pipex->cmd_args[i][0] == 0)
		pipex->exit_code = 126;
	else
		test_cmd_permission(pipex->path, pipex->cmd_args[i], pipex);
	if (redirect_stdin_stdout(i, &fd, pipex) == -1)
		ft_handle_error("dup2 error", pipex, &fd, 4);
	if (pipex->exit_code == 0)
	{
		execve(pipex->pathname, pipex->cmd_args[i], pipex->envp);
		pipex->exit_code = -1;
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	ft_free_pipex(pipex, &fd, 2);
	free(pipex->pid);
	exit(pipex->exit_code);
}
