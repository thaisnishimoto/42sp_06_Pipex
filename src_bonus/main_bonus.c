/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/04 12:39:25 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	exec_first_cmd(int i, t_fd fd, char *argv[], t_data *pipex, char *envp[])
{
	fd.infile = open(argv[1], O_RDONLY);
	if (fd.infile < 0)
	{
		ft_printf("%s: ", argv[1]);
		ft_handle_error("", pipex, &fd, 2);
	}
	pipex->cmd.args_count = ft_count_args(argv[2], ' ');
	ft_split_cmd(argv[2], ' ', &pipex->cmd);
	test_cmd_permission(pipex->path, &pipex->cmd);
	if (pipex->cmd.exit_code == 127)
		ft_printf("%s: command not found\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 126)
		ft_printf("%s: permission denied\n", pipex->cmd.args[0]);
	if (redirect_stdin_stdout(fd.infile, fd.pipe[i][1], &fd, pipex) == -1)
		ft_handle_error("dup2 error", pipex, &fd, 4);
	close(fd.infile);
	if (pipex->cmd.exit_code == 0)
	{
		execve(pipex->cmd.pathname, pipex->cmd.args, envp);
		pipex->cmd.exit_code = -1;
	}
	ft_free_matrix(pipex->path, pipex->path_count);
	ft_free_matrix(pipex->cmd.args, pipex->cmd.args_count);
	exit(pipex->cmd.exit_code);
}

void	exec_mid_cmd(int i, t_fd fd, char *argv[], t_data *pipex, char *envp[])
{
	pipex->cmd.args_count = ft_count_args(argv[i + 2], ' ');
	ft_split_cmd(argv[i + 2], ' ', &pipex->cmd);
	test_cmd_permission(pipex->path, &pipex->cmd);
	if (pipex->cmd.exit_code == 127)
		ft_printf("%s: command not found\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 126)
		ft_printf("%s: permission denied\n", pipex->cmd.args[0]);
	if (redirect_stdin_stdout(fd.pipe[i - 1][0], fd.pipe[i][1], &fd, pipex) == -1)
		ft_handle_error("dup2 error", pipex, &fd, 4);
	if (pipex->cmd.exit_code == 0)
	{
		execve(pipex->cmd.pathname, pipex->cmd.args, envp);
		pipex->cmd.exit_code = -1;
	}
	ft_free_matrix(pipex->path, pipex->path_count);
	ft_free_matrix(pipex->cmd.args, pipex->cmd.args_count);
	exit(pipex->cmd.exit_code);
}

void	exec_last_cmd(int i, t_fd fd, char *argv[], t_data *pipex, char *envp[])
{
	fd.outfile = open(argv[pipex->argc - 1], O_WRONLY | O_CREAT, 0777);
	if (fd.outfile < 0)
	{
		ft_printf("%s: ", argv[pipex->argc - 1]);
		ft_handle_error("", pipex, &fd, 2);
	}
	pipex->cmd.args_count = ft_count_args(argv[i + 2], ' ');
	ft_split_cmd(argv[i + 2], ' ', &pipex->cmd);
	test_cmd_permission(pipex->path, &pipex->cmd);
	if (pipex->cmd.exit_code == 127)
		ft_printf("%s: command not found\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 126)
		ft_printf("%s: permission denied\n", pipex->cmd.args[0]);
	if (redirect_stdin_stdout(fd.pipe[i - 1][0], fd.outfile, &fd, pipex) == -1)
		ft_handle_error("dup2 error", pipex, &fd, 4);
	close(fd.outfile);
	if (pipex->cmd.exit_code == 0)
	{
		execve(pipex->cmd.pathname, pipex->cmd.args, envp);
		pipex->cmd.exit_code = -1;
	}
	ft_free_matrix(pipex->path, pipex->path_count);
	ft_free_matrix(pipex->cmd.args, pipex->cmd.args_count);
	exit(pipex->cmd.exit_code);
}

int	redirect_stdin_stdout(int new_stdin, int new_stdout, t_fd *fd, t_data *pipex)
{
	if (dup2(new_stdin, STDIN_FILENO) == -1)
		return (-1);
	if (dup2(new_stdout, STDOUT_FILENO) == -1)
		return (-1);
	ft_close_pipes(fd->pipe, pipex->pipe_count);
	return (0);
}

//void	child_process(int i, t_fd *fd, char *argv, t_data *pipex, char *envp[]);
//{
//	if (i ==0)
//	fd.infile = open(argv[1], O_RDONLY);
//	if (fd.infile < 0)
//	{
//		ft_printf("%s: ", argv[1]);
//		ft_handle_error("", pipex, &fd, 2);
//	}
//	fd.outfile = open(argv[4], O_WRONLY | O_CREAT, 0777);
//	if (fd.outfile < 0)
//	{
//		ft_printf("%s: ", argv[4]);
//		ft_handle_error("", pipex, &fd, 2);
//	}
//}

void	create_pipes(int argc, t_fd *fd, t_data *pipex)
{
	int	i;

	pipex->argc = argc;
	pipex->cmd_count = argc - 3;
	pipex->pipe_count = pipex->cmd_count - 1;
	fd->pipe = malloc((pipex->pipe_count + 1) * sizeof(int *));
	if (fd->pipe == NULL)
		ft_handle_error("malloc for pipe ptr failed", pipex, fd, 1);
	fd->pipe[pipex->pipe_count] = NULL;
	i = 0;
	while (i < pipex->pipe_count)
	{
		fd->pipe[i] = malloc(2 * sizeof(int)); 
		if (fd->pipe[i] == NULL)
		{
			ft_close_pipes(fd->pipe, i);
			ft_handle_error("malloc for pipes failed", pipex, fd, 1);
		}
		if (pipe(fd->pipe[i]) == -1)
		{
			free(fd->pipe[i]);
			ft_close_pipes(fd->pipe, i);
			ft_handle_error("pipe error", pipex, fd, 1);
		}
		i++;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_fd	fd;
	t_data	pipex;
	int	i;

	handle_argc_and_envp(argc, envp, &pipex);
	create_pipes(argc, &fd, &pipex);
	pipex.pid = malloc(pipex.cmd_count * sizeof(int));
	if (pipex.pid == NULL)
		ft_handle_error("malloc for pid error", &pipex, &fd, 2);
	i = 0;
	while (i < pipex.cmd_count)
	{
		pipex.pid[i] = fork();
		if (pipex.pid[i] < 0)
			ft_handle_error("fork error", &pipex, &fd, 3);
		if (pipex.pid[i] == 0)
		{
			free(pipex.pid);
			if (i == 0)
				exec_first_cmd(i, fd, argv, &pipex, envp);
			else if (i < pipex.cmd_count -1)
				exec_mid_cmd(i, fd, argv, &pipex, envp);
			else
				exec_last_cmd(i, fd, argv, &pipex, envp);
//child_process(i, fd, argv, &pipex, envp);
		}
		i++;
	}
	wait_finish_pipe(&fd, &pipex);
	free(pipex.pid);
	return (pipex.exit_code);
}
