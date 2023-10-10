/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 23:07:52 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/10 18:15:07 by tmina-ni         ###   ########.fr       */
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
	int		argc;
	char	**envp;
	char	**path;
	int		path_count;
	char	***cmd_args;
	char	*pathname;
	int		cmd_count;
	int		cmd_offset;
	int		*args_count;
	int		pipe_count;
	pid_t	*pid;
	int		wstatus;
	int		exit_code;
	t_hdoc	heredoc;
}	t_data;

/*Main functions*/
void	check_argc_and_heredoc(int argc, char *argv[], t_data *pipe);
void	get_path(char *envp[], t_data *pipex);
void	get_cmd_args(char *argv[], t_data *pipex);
void	create_pipes(t_fd *fd, t_data *pipex);

/*Exec functions*/
void	exec_cmd(int i, t_fd fd, char *argv[], t_data *pipex);
void	open_infile(t_fd *fd, char *argv[], t_data *pipex, t_hdoc *heredoc);
void	open_outfile(t_fd *fd, char *argv[], t_data *pipex);
void	test_cmd_permission(char **path, char **cmd_args, t_data *pipex);
int		redirect_stdin_stdout(int i, t_fd *fd, t_data *pipex);

/*Split utils*/
char	**ft_split_args(const char *s, char c);
int		ft_count_args(const char *s, char c);
size_t	ft_arg_len(const char *str, char c);
char	*ft_trim_quotes(char *s1, const char *set);
size_t	ft_preserve_quoted_substr(const char *str);

/*Exit utils*/
void	ft_free_matrix(char **array, int size);
void	ft_close_pipes(int **pipe, int count);
void	ft_free_pipex(t_data *pipex, t_fd *fd, int stage);
void	ft_handle_error(char *error_msg, t_data *pipex, t_fd *fd, int stage);
void	wait_finish_pipe(t_fd *fd, t_data *pipex);

#endif
