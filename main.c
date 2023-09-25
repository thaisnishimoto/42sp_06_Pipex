/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/22 19:39:16 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_split_cmd(char const *s, char c, t_cmd *cmd);
char	*ft_add_path(char *path, char *cmd);

void	handle_args(int argc, char *argv[], char *envp[], t_cmd *cmd)
{
	int	i;

	i = 0;
	if (argc != 5)
	{
		ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n");
		exit(EXIT_FAILURE);
	}
	while (ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	ft_split_paths(envp[i] + 5, ':', cmd);
	cmd->count = argc - 3;
	i = 2;
	while (i < argc - 1)
	{
		ft_split_cmd(argv[i], ' ', cmd);
		i++;
	}
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

void	exec_first_cmd(t_fd fd, char *argv[], t_cmd cmd)
{
	int	i;
 
	fd.infile = open(argv[1], O_RDONLY);
	if (fd.infile < 0)
		ft_handle_perror("open infile error");
	close(fd.pipe[0]);
	redirect_stdin_stdout(fd.infile, fd.pipe[1]);
      	cmd = ft_strjoin("/usr/bin/", cmd[0]);
	while (cmd->path[i])
      	//cmd[0] = ft_strjoin("/usr/bin/", cmd[0]);
	execve(cmd->args[0], cmd->args, NULL); 
}

//void	exec_second_cmd(t_fd fd, char *argv[], t_cmd *cmd)
//{
//	int	i;
//
//	fd.outfile = open(argv[4], O_WRONLY | O_CREAT, 0777);
//	if (fd.outfile < 0)
//		ft_handle_perror("open outfile error");
//	close(fd.pipe[1]);
//	redirect_stdin_stdout(fd.pipe[0], fd.outfile);
//	//cmd = ft_split_cmd(argv[3], ' ');
//	//cmd[0] = ft_add_path("/usr/bin/", cmd[0]);
//	//if (access(cmd[0], X_OK) == -1)
//	//	exit(126);
//	execve(cmd->args[0], cmd->args, NULL); 
//	i = 0;
//	while (cmd[i])
//		free(cmd[i++]);
//	free(cmd);
////	exit (127);
//}

int	main(int argc, char *argv[], char *envp[])
{
	t_fd	fd;
	t_fork	process;
	t_cmd	cmd;

	handle_args(argc, argv, envp, &cmd);
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
//	if (process.id2 == 0)
//		exec_second_cmd(fd, argv);
	ft_free_matrix(cmd->path);
	ft_free_matrix(cmd->args);
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
