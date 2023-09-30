/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 15:58:47 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/30 11:59:01 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	exec_first_cmd(t_fd fd, char *argv[], t_data *pipex, char *envp[])
{
	fd.infile = open(argv[1], O_RDONLY);
	if (fd.infile < 0)
	{
		ft_free_matrix(pipex->path, pipex->path_count);
		ft_printf("%s: ", argv[1]);
		ft_handle_perror("");
	}
	close(fd.pipe[0]);
	pipex->cmd.args_count = ft_count_args(argv[2], ' ');
	ft_split_cmd(argv[2], ' ', &pipex->cmd);
	test_cmd_permission(pipex->path, &pipex->cmd);
	ft_printf("cmd: %s", pipex->cmd.pathname);
	if (pipex->cmd.exit_code == 127)
		ft_printf("%s: command not found\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 126)
		ft_printf("%s: permission denied\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 0)
	{
		redirect_stdin_stdout(fd.infile, fd.pipe[1]);
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
		ft_free_matrix(pipex->path, pipex->path_count);
		ft_printf("%s: ", argv[4]);
		ft_handle_perror("");
	}
	close(fd.pipe[1]);
	pipex->cmd.args_count = ft_count_args(argv[3], ' ');
	ft_split_cmd(argv[3], ' ', &pipex->cmd);
	test_cmd_permission(pipex->path, &pipex->cmd);
	if (pipex->cmd.exit_code == 127)
		ft_printf("%s: command not found\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 126)
		ft_printf("%s: permission denied\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 0)
	{
		redirect_stdin_stdout(fd.pipe[0], fd.outfile);
		execve(pipex->cmd.pathname, pipex->cmd.args, envp);
		pipex->cmd.exit_code = -1;
	}
	ft_free_matrix(pipex->path, pipex->path_count);
	ft_free_matrix(pipex->cmd.args, pipex->cmd.args_count);
	exit(pipex->cmd.exit_code);
}

void	redirect_stdin_stdout(int new_stdin, int new_stdout)
{
	if (dup2(new_stdin, STDIN_FILENO) == -1)
		ft_handle_perror("dup2 error");
	close(new_stdin);
	if (dup2(new_stdout, STDOUT_FILENO) == -1)
		ft_handle_perror("dup2 error");
	close(new_stdout);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_fd	fd;
	t_fork	process;
	t_data	pipex;
	int	i;

	handle_argc_and_envp(argc, envp, &pipex);
	fd.pipe = malloc((pipex.pipe_count + 1) * sizeof(int *));
	if (fd.pipe == NULL)
		ft_handle_error("malloc for pipe pointers failed");
	fd.pipe[pipex.pipe_count] = NULL;
	i = 0;
	while (i < pipex.pipe_count)
	{
		fd.pipe[i] = malloc(2 * sizeof(int)); 
		if (fd.pipe[i] == NULL)
		{
			ft_free_matrix(fd.pipe, --i);
			ft_handle_error("malloc for pipes failed");
		}
		if (pipe(fd.pipe[i]) == -1)
		{
			ft_close_pipe(fd.pipe, --i);
			ft_free_matrix(fd.pipe, --i);
			ft_handle_perror("pipe error");
		}
		i++;
	}
	i = 0;
	while (i < pipex->cmd_count)
	{
		if (pipe(fd.pipe[i]) == -1)
		{
			ft_free_matrix(fd.pipe, pipex.pipe_count);
			ft_handle_perror("pipe error");
		}
		process.id[i] = fork();
		if (process.id[i] < 0)
		{
			ft_close_pipe(&fd);
			ft_free_matrix(fd.pipe, pipex.pipe_count);
			ft_handle_perror("fork error");
		}
		if (process.id[i] == 0)
			exec_first_cmd(fd, argv[i + 2], &pipex, envp);
//	process.id2 = fork();
//	if (process.id2 < 0)
//	{
//		ft_close_pipe(&fd);
//		ft_handle_perror("fork2 error");
//	}
//	if (process.id2 == 0)
//		exec_second_cmd(fd, argv, &pipex, envp);
	wait_finish_pipe(&fd, &process, &pipex);
	return (process.exit_code);
}
