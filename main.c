/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/19 19:45:19 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_split_cmd(char const *s, char c);
char	*ft_add_path(char *path, char *cmd);

typedef struct s_fd
{
	int	infile;
	int	outfile;
	int	pipe[2];
}	t_fd;

void	handle_error(char *function_name)
{
	perror(function_name);
	exit(EXIT_FAILURE);
}

void	validate_args(int argc, char *argv[], t_fd *fd)
{
	if (argc != 5)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
		exit(EXIT_FAILURE);
	}
	fd->infile = open(argv[1], O_RDONLY);
	if (fd->infile < 0)
		handle_error("open infile error");
	fd->outfile = open(argv[4], O_WRONLY | O_CREAT, 0777);
	if (fd->outfile < 0)
		handle_error("open outfile error");
}

void	redirect_stdin_stdout(int new_stdin, int new_stdout)
{
	if (dup2(new_stdin, STDIN_FILENO) == -1)
		handle_error("dup2 error");
	close(new_stdin);
	if (dup2(new_stdout, STDOUT_FILENO) == -1)
		handle_error("dup2 error");
	close(new_stdout);
}

void	exec_first_cmd(t_fd fd, char *argv[])
{
	char	**cmd;
	int	i;
 
	fd.infile = open(argv[1], O_RDONLY);
	if (fd.infile < 0)
		handle_error("open infile error");
	close(fd.pipe[0]);
	redirect_stdin_stdout(fd.infile, fd.pipe[1]);
	cmd = ft_split_cmd(argv[2], ' ');
      	cmd[0] = ft_add_path("/usr/bin/", cmd[0]);
	execve(cmd[0], cmd, NULL);
	i = 0;
	while (cmd[i])
		free(cmd[i++]);
	free(cmd);
	exit (127);
}

void	exec_second_cmd(t_fd fd, char *argv[])
{
	char	**cmd;
	int	i;

	fd.outfile = open(argv[4], O_WRONLY | O_CREAT, 0777);
	if (fd.outfile < 0)
		handle_error("open outfile error");
	close(fd.pipe[1]);
	redirect_stdin_stdout(fd.pipe[0], fd.outfile);
	cmd = ft_split_cmd(argv[3], ' ');
	cmd[0] = ft_add_path("/usr/bin/", cmd[0]);
	execve(cmd[0], cmd, NULL); 
	i = 0;
	while (cmd[i])
		free(cmd[i++]);
	free(cmd);
	exit (127);
}

int	main(int argc, char *argv[])
{
	t_fd	fd;
	pid_t	pid1;
	pid_t	pid2;
	int	wstatus1;
	int	wstatus2;
	int	status_code;

	fd.infile = open("file_x", O_RDONLY | O_CREAT, 0000);
	if (argc != 5)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
		exit(EXIT_FAILURE);
	}
	if (pipe(fd.pipe) == -1)
		handle_error("pipe1 error");
	pid1 = fork();
	if (pid1 < 0)
	{
		close(fd.pipe[0]);
		close(fd.pipe[1]);
		handle_error("fork1 error");
	}
	if (pid1 == 0)
		exec_first_cmd(fd, argv);
	pid2 = fork();
	if (pid2 < 0)
	{
		close(fd.pipe[0]);
		close(fd.pipe[1]);
		handle_error("fork2 error");
	}
	if (pid2 == 0)
		exec_second_cmd(fd, argv);
	close(fd.pipe[0]);
	close(fd.pipe[1]);
	waitpid(pid1, &wstatus1, 0);
	waitpid(pid2, &wstatus2, 0);
	if (WIFEXITED(wstatus2))
	{
		status_code = WEXITSTATUS(wstatus2);
		if (status_code == 127)
			ft_printf("%s %s: command not found\n", argv[1], argv[3]);
	}
	return (status_code);
}
