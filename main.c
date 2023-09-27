/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/27 02:12:13 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_split_cmd(char const *s, char c, t_cmd *cmd);
char	*ft_add_path(char *path, char *cmd);

//void	handle_args(int argc, char *argv[], char *envp[], t_data *pipe)
//{
//	int	i;
//
//	i = 0;
//	if (argc != 5)
//	{
//		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
//		exit(EXIT_FAILURE);
//	}
//	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
//		i++;
//	ft_split_paths(envp[i] + 5, ':', pipe);
//	pipe->cmd_count = argc - 3;
//	ft_split_cmd(argv[2], ' ', &pipe->cmd1);
//	ft_split_cmd(argv[3], ' ', &pipe->cmd2);
//
//}

void	redirect_stdin_stdout(int new_stdin, int new_stdout)
{
	if (dup2(new_stdin, STDIN_FILENO) == -1)
		ft_handle_perror("dup2 error");
	close(new_stdin);
	if (dup2(new_stdout, STDOUT_FILENO) == -1)
		ft_handle_perror("dup2 error");
	close(new_stdout);
}

void	test_cmd_permission(char **path, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (path[i])
	{
		cmd->exit_code = 0;
		cmd->pathname = ft_strjoin(path[i], cmd->args[0]);
		if (access(cmd->pathname, F_OK) == -1)
		{
			cmd->exit_code = 127;
			free(cmd->pathname);
			i++;
		}
		else if (access(cmd->pathname, X_OK) == -1)
		{
			cmd->exit_code = 126;
			free(cmd->pathname);
			break ;
		}
		else
			break ;
	}
}

void	exec_first_cmd(t_fd fd, char *argv[], t_data *pipex, char *envp[])
{
	fd.infile = open(argv[1], O_RDONLY);
	if (fd.infile < 0)
	{
		ft_free_matrix(pipex->path, pipex->path_count);
		ft_handle_perror("open infile error");
	}
	close(fd.pipe[0]);
	pipex->cmd.args_count = ft_count_args(argv[2], ' ');
	ft_split_cmd(argv[2], ' ', &pipex->cmd);
	test_cmd_permission(pipex->path, &pipex->cmd);
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
		ft_handle_perror("open outfile error");
	}
	close(fd.pipe[1]);
	pipex->cmd.args_count = ft_count_args(argv[3], ' ');
	ft_split_cmd(argv[3], ' ', &pipex->cmd);
	test_cmd_permission(pipex->path, &pipex->cmd);
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

int	main(int argc, char *argv[], char *envp[])
{
	t_fd	fd;
	t_fork	process;
	t_data	pipex;
	int	i;

	i = 0;
	if (argc != 5)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
		exit(EXIT_FAILURE);
	}
	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	pipex.path_count = ft_count_args(envp[i], ':');
	ft_split_paths(envp[i] + 5, ':', &pipex);
	if (pipe(fd.pipe) == -1)
		ft_handle_perror("pipe error");
	process.id1 = fork();
	if (process.id1 < 0)
	{
		ft_close_pipe(&fd);
		ft_handle_perror("fork1 error");
	}
	if (process.id1 == 0)
		exec_first_cmd(fd, argv, &pipex, envp);
	process.id2 = fork();
	if (process.id2 < 0)
	{
		ft_close_pipe(&fd);
		ft_handle_perror("fork2 error");
	}
	if (process.id2 == 0)
		exec_second_cmd(fd, argv, &pipex, envp);
	wait_finish_pipe(&fd, &process);
	ft_free_matrix(pipex.path, pipex.path_count);
	return (process.exit_code);
}
