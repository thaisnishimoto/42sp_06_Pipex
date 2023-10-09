/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/09 00:55:17 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	check_if_heredoc(char *argv[], t_data *pipex)
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
	{
		pipex->heredoc.flag = 1;
		pipex->heredoc.eof = argv[2];
		pipex->heredoc.eof_len = ft_strlen(argv[2]);
		pipex->cmd_offset = 3;
	}
	else
	{
		pipex->heredoc.flag = 0;
		pipex->cmd_offset = 2;
	}
}

void	handle_argc_and_envp(int argc, char *envp[], t_data *pipex)
{
	int	i;

	i = 0;
	if (pipex->heredoc.flag == 1 && argc < 5)
	{
		ft_printf("Usage: ./pipex here_doc LIMITER cmd1 [cmd2 ...] outfile\n");
		exit(EXIT_FAILURE);
	}
	else if (pipex->heredoc.flag == 0 && argc < 4)
	{
		ft_printf("Usage: ./pipex infile cmd1 [cmd2 ...] outfile\n");
		exit(EXIT_FAILURE);
	}
	pipex->argc = argc;
	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	pipex->path_count = ft_count_args(envp[i] + 5, ':');
	pipex->path = ft_split_args(envp[i] + 5, ':');
	pipex->envp = envp;
}

void	create_pipes(int argc, t_fd *fd, t_data *pipex)
{
	int	i;

	pipex->cmd_count = argc - 1 - pipex->cmd_offset;
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
			ft_handle_error("malloc for pipe fd failed", pipex, fd, 1);
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

void	exec_cmd(int i, t_fd fd, char *argv[], t_data *pipex)
{
	if (i == 0)
		open_infile(&fd, argv, pipex, &pipex->heredoc);
	if (i == pipex->cmd_count - 1)
		open_outfile(&fd, argv, pipex);
	pipex->cmd.args_count = ft_count_args(argv[i+ pipex->cmd_offset], ' ');
	pipex->cmd.args = ft_split_args(argv[i + pipex->cmd_offset], ' ');
	test_cmd_permission(pipex->path, &pipex->cmd);
	if (pipex->cmd.exit_code == 127)
		ft_printf("%s: command not found\n", pipex->cmd.args[0]);
	if (pipex->cmd.exit_code == 126)
		ft_printf("%s: permission denied\n", pipex->cmd.args[0]);
	if (redirect_stdin_stdout(i, &fd, pipex) == -1)
		ft_handle_error("dup2 error", pipex, &fd, 4);
	if (pipex->cmd.exit_code == 0)
	{
		execve(pipex->cmd.pathname, pipex->cmd.args, pipex->envp);
		pipex->cmd.exit_code = -1;
	}
	ft_free_matrix(pipex->path, pipex->path_count);
	ft_free_matrix(pipex->cmd.args, pipex->cmd.args_count);
	free(pipex->pid);
	exit(pipex->cmd.exit_code);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	pipex;
	t_fd	fd;
	int		i;

	check_if_heredoc(argv, &pipex);
	handle_argc_and_envp(argc, envp, &pipex);
	create_pipes(argc, &fd, &pipex);
	pipex.pid = malloc(pipex.cmd_count * sizeof(int));
	if (pipex.pid == NULL)
		ft_handle_error("malloc for pid error", &pipex, &fd, 2);
	i = 0;
	while (i < pipex.cmd_count)
	{
//		ft_split_cmd222222222222(argv[i + pipex->cmd_offset], ' ', pipex, &fd);
		pipex.pid[i] = fork();
		if (pipex.pid[i] < 0)
			ft_handle_error("fork error", &pipex, &fd, 3);
		if (pipex.pid[i] == 0)
			exec_cmd(i, fd, argv, &pipex);
		i++;
	}
	wait_finish_pipe(&fd, &pipex);
	return (pipex.exit_code);
}
