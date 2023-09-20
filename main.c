/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/20 19:15:04 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_split_cmd(char const *s, char c);
char	*ft_add_path(char *path, char *cmd);

void	validate_args(int argc, char *argv[], t_fd *fd)
{
	if (argc != 5)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
		exit(EXIT_FAILURE);
	}
	fd->infile = open(argv[1], O_RDONLY);
	if (fd->infile < 0)
		ft_handle_perror("open infile error");
	fd->outfile = open(argv[4], O_WRONLY | O_CREAT, 0777);
	if (fd->outfile < 0)
		ft_handle_perror("open outfile error");
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

void	exec_first_cmd(t_fd fd, char *argv[])
{
	char	**cmd;
	int	i;
 
	fd.infile = open(argv[1], O_RDONLY);
	if (fd.infile < 0)
		ft_handle_perror("open infile error");
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
		ft_handle_perror("open outfile error");
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
	t_fork	process;

	if (argc != 5)
		ft_handle_error("Usage: ./pipex infile cmd1 cmd2 outfile\n");
	if (pipe(fd.pipe) == -1)
		ft_handle_perror("pipe error");
	process.id1 = fork();
	if (process.id1 < 0)
	{
		ft_close_pipe(&fd);
		ft_handle_perror("fork1 error");
	}
	if (process.id1 == 0)
		exec_first_cmd(fd, argv);
	process.id2 = fork();
	if (process.id2 < 0)
	{
		ft_close_pipe(&fd);
		ft_handle_perror("fork2 error");
	}
	if (process.id2 == 0)
		exec_second_cmd(fd, argv);
	wait_finish_pipe(&fd, &process);
//	ft_close_pipe(&fd.pipe);
//	waitpid(pid1, &process.wstatus1, 0);
//	waitpid(pid2, &process.wstatus2, 0);
//	if (WIFEXITED(process.wstatus2))
//	{
//		process.exit_code = WEXITSTATUS(process.wstatus2);
//		if (process.exit_code == 127)
//			ft_printf("%s %s: command not found\n", argv[1], argv[3]);
//	}
	return (process.exit_code);
}
