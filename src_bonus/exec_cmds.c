/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/04 20:50:00 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	open_in_out_file(int i, t_fd *fd, char *argv[], t_data *pipex)
{
	if (i == 0)
	{
		fd->infile = open(argv[1], O_RDONLY);
		if (fd->infile < 0)
		{
			ft_printf("%s: ", argv[1]);
			ft_handle_error("", pipex, fd, 3);
		}
	}
	else if (i == pipex->cmd_count - 1)
	{
		fd->outfile = open(argv[pipex->argc - 1], O_WRONLY | O_CREAT, 0777);
		if (fd->outfile < 0)
		{
			ft_printf("%s: ", argv[pipex->argc - 1]);
			ft_handle_error("", pipex, fd, 3);
		}
	}
}

void	ft_split_cmd(char *s, char c, t_data *pipex, t_fd *fd)
{
	int		i;
	int		j;

	pipex->cmd.args_count = ft_count_args(s, ' ');
	pipex->cmd.args = malloc((pipex->cmd.args_count + 1) * sizeof(char *));
	if (pipex->cmd.args == NULL)
		ft_handle_error("malloc failed", pipex, fd, 3);
	i = 0;
	j = 0;
	while (j < pipex->cmd.args_count)
	{
		while (s[i] == c)
			i++;
		pipex->cmd.args[j] = malloc((ft_arg_len(&s[i], c) + 1) * sizeof(char));
		if (pipex->cmd.args[j] == NULL)
		{
			ft_free_matrix(pipex->cmd.args, j);
			ft_handle_error("malloc failed", pipex, fd, 3);
		}
		ft_strlcpy(pipex->cmd.args[j], &s[i], ft_arg_len(&s[i], c) + 1);
		pipex->cmd.args[j] = ft_trim_quotes(pipex->cmd.args[j], "\'\"");
		i = i + ft_arg_len(&s[i], c);
		j++;
	}
	pipex->cmd.args[j] = NULL;
}

void	test_cmd_permission(char **path, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (path[i])
	{
		cmd->exit_code = 0;
		if (cmd->args[0][0] == '/')
			cmd->pathname = ft_str_rm_dup(cmd->args[0], '/');
		else
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
			return ;
	}
}

int	redirect_stdin_stdout(int i, t_fd *fd, t_data *pipex)
{
	if (i == 0)
	{
		if (dup2(fd->infile, STDIN_FILENO) == -1)
			return (-1);
		close(fd->infile);
	}
	else
	{
		if (dup2(fd->pipe[i - 1][0], STDIN_FILENO) == -1)
			return (-1);
	}
	if (i == pipex->cmd_count -1)
	{
		if (dup2(fd->outfile, STDOUT_FILENO) == -1)
			return (-1);
		close(fd->outfile);
	}
	else
	{
		if (dup2(fd->pipe[i][1], STDOUT_FILENO) == -1)
			return (-1);
	}
	ft_close_pipes(fd->pipe, pipex->pipe_count);
	return (0);
}

void	exec_cmd(int i, t_fd fd, char *argv[], t_data *pipex)
{
	open_in_out_file(i, &fd, argv, pipex);
	pipex->cmd.args_count = ft_count_args(argv[i + 2], ' ');
	ft_split_cmd(argv[i + 2], ' ', pipex, &fd);
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
