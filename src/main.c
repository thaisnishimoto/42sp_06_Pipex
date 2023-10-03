/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/03 16:37:27 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	exec_first_cmd(t_fd fd, char *argv[], t_data *pipex, char *envp[])
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
	if (redirect_stdin_stdout(fd.infile, fd.pipe[1], &fd) == -1)
		ft_handle_error("dup2 error", pipex, &fd, 3);
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

void	exec_second_cmd(t_fd fd, char *argv[], t_data *pipex, char *envp[])
{
	fd.outfile = open(argv[4], O_WRONLY | O_CREAT, 0777);
	if (fd.outfile < 0)
	{
		ft_printf("%s: ", argv[4]);
		ft_handle_error("", pipex, &fd, 2);
	}
	pipex->cmd.args_count = ft_count_args(argv[3], ' ');
	ft_split_cmd(argv[3], ' ', &pipex->cmd);
	test_cmd_permission(pipex->path, &pipex->cmd);
	if (pipex->cmd.exit_code == 127)
		ft_printf("%s: command not found\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 126)
		ft_printf("%s: permission denied\n", pipex->cmd.args[0]);
	if (redirect_stdin_stdout(fd.pipe[0], fd.outfile, &fd) == -1)
		ft_handle_error("dup2 error", pipex, &fd, 3);
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

int	redirect_stdin_stdout(int new_stdin, int new_stdout, t_fd *fd)
{
	if (dup2(new_stdin, STDIN_FILENO) == -1)
		return (-1);
	if (dup2(new_stdout, STDOUT_FILENO) == -1)
		return (-1);
	ft_close_pipe(fd);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_fd	fd;
	t_fork	process;
	t_data	pipex;

	handle_argc_and_envp(argc, envp, &pipex);
	if (pipe(fd.pipe) == -1)
		ft_handle_error("pipe error", &pipex, &fd, 1);
	process.id1 = fork();
	if (process.id1 < 0)
		ft_handle_error("fork1 error", &pipex, &fd, 2);
	if (process.id1 == 0)
		exec_first_cmd(fd, argv, &pipex, envp);
	process.id2 = fork();
	if (process.id2 < 0)
		ft_handle_error("fork2 error", &pipex, &fd, 2);
	if (process.id2 == 0)
		exec_second_cmd(fd, argv, &pipex, envp);
	wait_finish_pipe(&fd, &process, &pipex);
	return (process.exit_code);
}
