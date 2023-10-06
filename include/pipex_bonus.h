/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 23:07:52 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/06 13:04:47 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h> //pipe, dup, access, execve, fork
# include <stdlib.h> //malloc, exit
# include <fcntl.h> //open
# include <sys/types.h> //pid_t
# include <sys/wait.h> //wait
# include <errno.h> //perror
# include "../libft/libft.h"
# include <stdbool.h>

typedef struct s_fd
{
	int	infile;
	int	outfile;
	int	**pipe;
}	t_fd;

typedef struct s_cmd
{
	char	*pathname;
	char	**args;
	int		args_count;
	int		exit_code;
}	t_cmd;

typedef struct s_hdoc
{
	int		flag;
	char	*eof;
	int		eof_len;
	char	*line;
	int		pipe[2];
}	t_hdoc;

typedef struct s_data
{
	char	**path;
	int		path_count;
	int		cmd_count;
	int		pipe_count;
	int		argc;
	t_cmd	cmd;
	pid_t	*pid;
	int		wstatus;
	int		exit_code;
	char	**envp;
	t_hdoc	heredoc;
	int		cmd_offset;
}	t_data;

/*Main functions*/
void	handle_argc_and_envp(int argc, char *envp[], t_data *pipe);
void	ft_split_paths(char const *s, char c, t_data *pipe);
void	create_pipes(int argc, t_fd *fd, t_data *pipex);

/*Exec functions*/
void	open_infile(t_fd *fd, char *argv[], t_data *pipex, t_hdoc *heredoc);
void	open_outfile(t_fd *fd, char *argv[], t_data *pipex);
void	ft_split_cmd(char *s, char c, t_data *pipex, t_fd *fd);
void	test_cmd_permission(char **path, t_cmd *cmd);
int		redirect_stdin_stdout(int i, t_fd *fd, t_data *pipex);
void	exec_cmd(int i, t_fd fd, char *argv[], t_data *pipex);

/*Split utils*/
int		ft_count_args(const char *s, char c);
size_t	ft_arg_len(const char *str, char c);
char	*ft_trim_quotes(char *s1, const char *set);

/*Exit utils*/
void	ft_free_matrix(char **array, int j);
void	ft_close_pipes(int **pipe, int count);
void	ft_handle_error(char *error_msg, t_data *pipex, t_fd *fd, int stage);
void	wait_finish_pipe(t_fd *fd, t_data *pipex);

#endif
