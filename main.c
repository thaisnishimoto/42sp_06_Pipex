/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/05 18:44:28 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	redirect_fds(char *argv[])
{
	int	fd_in;
	int	fd_out;

	//dup(0);
	//dup(1);
	fd_in = open(argv[1], O_WRONLY | O_CREAT);
	fd_out = open(argv[4], O_WRONLY | O_CREAT);
	dup2(fd_in, 0);
	dup2(fd_out, 1);
}

int	main(int argc, char *argv[])
{
	int	fd_in;
	int	fd_out;
	int	fd[2];
	pid_t	pid;
	char	**cmd;

	if (argc < 5)
		ft_printf("arguments missing\n");
	fd_in = open(argv[1], O_RDONLY | O_CREAT);
	fd_out = open(argv[4], O_WRONLY | O_CREAT);
	pipe(fd);
	pid = fork();
	if (pid < 0)
		ft_printf("error");
	if (pid == 0)
	{
		printf("starting child process.\n");
		close(fd[0]);
		cmd = ft_split(argv[2], ' ');
	      	cmd[0] = ft_strjoin("/usr/bin/", cmd[0]);
		dup2(fd_in, 0);
		dup2(fd[1], 1);
      		if (execve(cmd[0], cmd, NULL) == -1)
			perror("execve error");
	}
	if (pid > 0)
	{
		printf("starting parent process.\n");
		close(fd[1]);
		wait(NULL);
		cmd = ft_split(argv[3], ' ');
	      	cmd[0] = ft_strjoin("/usr/bin/", cmd[0]);
		dup2(fd[0], 0);
		dup2(fd_out, 1);
		if (execve(cmd[0], cmd, NULL) == -1)
			perror("execve error");
		printf("end of parent process.\n");
	}
	close(fd_out);
	return (0);
}
