/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/09 22:48:12 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	check_argc_and_heredoc(int argc, char *argv[], t_data *pipex)
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
	{
		if (argc < 6)
		{
			ft_printf("Usage: ./pipex here_doc EOF cmd1 cmd2 [cmdN] outfile\n");
			exit(EXIT_FAILURE);
		}
		pipex->heredoc.flag = 1;
		pipex->heredoc.eof = argv[2];
		pipex->heredoc.eof_len = ft_strlen(argv[2]);
		pipex->cmd_offset = 3;
	}
	else
	{
		if (argc < 5)
		{
			ft_printf("Usage: ./pipex infile cmd1 cmd2 [cmd3 ...] outfile\n");
			exit(EXIT_FAILURE);
		}
		pipex->heredoc.flag = 0;
		pipex->cmd_offset = 2;
	}
	pipex->argc = argc;
}

void	get_path(char *envp[], t_data *pipex)
{
	int		i;
	char	*temp;

	i = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	pipex->path_count = ft_count_args(envp[i] + 5, ':');
	pipex->path = ft_split_args(envp[i] + 5, ':');
	if (pipex->path == NULL)
		ft_handle_error("split path failed", NULL, NULL, 0);
	i = 0;
	while (i < pipex->path_count)
	{
		temp = ft_strjoin(pipex->path[i], "/");
		free(pipex->path[i]);
		pipex->path[i] = ft_strdup(temp);
		free(temp);
		i++;
	}
	pipex->envp = envp;
}

void	get_cmd_args(char *argv[], t_data *pipex)
{
	int	i;

	pipex->cmd_count = pipex->argc - 1 - pipex->cmd_offset;
	pipex->cmd_args = ft_calloc((pipex->cmd_count + 1), sizeof(char **));
	if (pipex->cmd_args == NULL)
		ft_handle_error("cmd calloc failed", pipex, NULL, 1);
	pipex->args_count = ft_calloc((pipex->cmd_count + 1), sizeof(int));
	if (pipex->args_count == NULL)
	{
		free(pipex->cmd_args);
		ft_handle_error("args count calloc failed", pipex, NULL, 1);
	}
	i = 0;
	while (i < pipex->cmd_count)
	{
		pipex->args_count[i] = ft_count_args(argv[i + pipex->cmd_offset], ' ');
		pipex->cmd_args[i] = ft_split_args(argv[i + pipex->cmd_offset], ' ');
		if (pipex->path == NULL)
		{
			free(pipex->cmd_args);
			free(pipex->args_count);
			ft_handle_error("split cmd failed", pipex, NULL, 1);
		}
		i++;
	}
}

void	create_pipes(t_fd *fd, t_data *pipex)
{
	int	i;

	pipex->pipe_count = pipex->cmd_count - 1;
	fd->pipe = malloc((pipex->pipe_count + 1) * sizeof(int *));
	if (fd->pipe == NULL)
		ft_handle_error("malloc for pipe ptr failed", pipex, fd, 2);
	fd->pipe[pipex->pipe_count] = NULL;
	i = 0;
	while (i < pipex->pipe_count)
	{
		fd->pipe[i] = malloc(2 * sizeof(int));
		if (fd->pipe[i] == NULL)
		{
			ft_close_pipes(fd->pipe, i);
			ft_handle_error("malloc for pipe fd failed", pipex, fd, 2);
		}
		if (pipe(fd->pipe[i]) == -1)
		{
			free(fd->pipe[i]);
			ft_close_pipes(fd->pipe, i);
			ft_handle_error("pipe error", pipex, fd, 2);
		}
		i++;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	pipex;
	t_fd	fd;
	int		i;

	check_argc_and_heredoc(argc, argv, &pipex);
	get_path(envp, &pipex);
	get_cmd_args(argv, &pipex);
	create_pipes(&fd, &pipex);
	pipex.pid = malloc(pipex.cmd_count * sizeof(int));
	if (pipex.pid == NULL)
		ft_handle_error("malloc for pid error", &pipex, &fd, 3);
	i = 0;
	while (i < pipex.cmd_count)
	{
		pipex.pid[i] = fork();
		if (pipex.pid[i] < 0)
			ft_handle_error("fork error", &pipex, &fd, 4);
		if (pipex.pid[i] == 0)
			exec_cmd(i, fd, argv, &pipex);
		i++;
	}
	wait_finish_pipe(&fd, &pipex);
	return (pipex.exit_code);
}
