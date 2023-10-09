/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/09 00:09:17 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	open_infile(t_fd *fd, char *argv[], t_data *pipex, t_hdoc *heredoc)
{
	if (heredoc->flag == 0)
	{
		fd->infile = open(argv[1], O_RDONLY);
		if (fd->infile < 0)
		{
			ft_printf("%s: ", argv[1]);
			ft_handle_error("", pipex, fd, 3);
		}
	}
	else if (heredoc->flag == 1)
	{
		pipe(heredoc->pipe);
		fd->infile = heredoc->pipe[0];
		ft_printf(">");
		heredoc->line = ft_get_next_line(STDIN_FILENO);
		while (ft_strncmp(heredoc->line, heredoc->eof, heredoc->eof_len) != 0)
		{
			ft_putstr_fd(heredoc->line, heredoc->pipe[1]);
			free(heredoc->line);
			ft_printf(">");
			heredoc->line = ft_get_next_line(STDIN_FILENO);
		}
		free(heredoc->line);
		close(heredoc->pipe[1]);
	}
}

void	open_outfile(t_fd *fd, char *argv[], t_data *pipex)
{
	char	*outfile_path;

	outfile_path = argv[pipex->argc -1];
	if (pipex->heredoc.flag == 0)
	{
		fd->outfile = open(outfile_path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if (fd->outfile < 0)
		{
			ft_printf("%s: ", argv[pipex->argc - 1]);
			ft_handle_error("", pipex, fd, 3);
		}
	}
	else if (pipex->heredoc.flag == 1)
	{
		fd->outfile = open(outfile_path, O_WRONLY | O_APPEND | O_CREAT, 0777);
		if (fd->outfile < 0)
		{
			ft_printf("%s: ", argv[pipex->argc - 1]);
			ft_handle_error("", pipex, fd, 3);
		}
	}
}

void	test_cmd_permission(char **path, t_cmd *cmd)
{
	int	i;
	char	*temp;

	i = 0;
	while (path[i])
	{
		cmd->exit_code = 0;
		if (cmd->args[0][0] == '/')
			cmd->pathname = ft_str_rm_dup(cmd->args[0], '/');
		else
		{
			temp = ft_strjoin(path[i], "/");
			cmd->pathname = ft_strjoin(temp, cmd->args[0]);
			free(temp);
			ft_printf("pathname: %s\n", cmd->pathname);
		}
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
	if (i == pipex->cmd_count - 1)
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
