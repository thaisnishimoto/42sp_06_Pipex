/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/04 20:48:48 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	handle_argc_and_envp(int argc, char *envp[], t_data *pipex)
{
	int	i;

	i = 0;
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 [cmd3 ...] outfile\n");
		exit(EXIT_FAILURE);
	}
	pipex->argc = argc;
	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	ft_split_paths(envp[i] + 5, ':', pipex);
	pipex->envp = envp;
}

void	ft_split_paths(char const *s, char c, t_data *pipex)
{
	int		i;
	int		j;

	pipex->path_count = ft_count_args(s, ':');
	pipex->path = malloc((pipex->path_count + 1) * sizeof(char *));
	if (pipex->path == NULL)
		ft_handle_error("malloc failed", NULL, NULL, 0);
	i = 0;
	j = 0;
	while (j < pipex->path_count)
	{
		while (s[i] == c)
			i++;
		pipex->path[j] = malloc((ft_arg_len(&s[i], c) + 2) * sizeof(char));
		if (pipex->path[j] == NULL)
		{
			ft_free_matrix(pipex->path, j);
			ft_handle_error("malloc failed", NULL, NULL, 0);
		}
		ft_strlcpy(pipex->path[j], &s[i], ft_arg_len(&s[i], c) + 1);
		ft_strlcat(pipex->path[j], "/", ft_arg_len(&s[i], c) + 2);
		i = i + ft_arg_len(&s[i], c);
		j++;
	}
	pipex->path[j] = NULL;
}

void	create_pipes(int argc, t_fd *fd, t_data *pipex)
{
	int	i;

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

int	main(int argc, char *argv[], char *envp[])
{
	t_data	pipex;
	t_fd	fd;
	int		i;

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
			exec_cmd(i, fd, argv, &pipex);
		i++;
	}
	wait_finish_pipe(&fd, &pipex);
	return (pipex.exit_code);
}
