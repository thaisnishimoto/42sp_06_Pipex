/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 23:07:52 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/04 12:38:22 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h> //pipe, dup, access, execve, fork
# include <stdlib.h> //malloc, exit
# include <fcntl.h> //open
# include <sys/types.h> //pid_t
# include <sys/wait.h> //wait
# include <errno.h> //perror
# include "../libft/libft.h"

typedef struct s_fd
{
	int	infile;
	int	outfile;
	int	**pipe;
}	t_fd;

//typedef struct s_fork
//{
//	pid_t	*id;
//	int	wstatus;
//	int		exit_code;
//}	t_fork;

typedef struct s_cmd
{
	char	*pathname;
	char	**args;
	int		args_count;
	int		exit_code;
}	t_cmd;

typedef struct s_data
{
	char	**path;
	int		path_count;
	int		cmd_count;
	int		pipe_count;
	int		argc;
	t_cmd	cmd;
	pid_t	*pid;
	int	wstatus;
	int		exit_code;
}	t_data;

/*Pipe functions*/
void	exec_first_cmd(int i, t_fd fd, char *argv[], t_data *pipex, char *envp[]);
void	exec_mid_cmd(int i, t_fd fd, char *argv[], t_data *pipex, char *envp[]);
void	exec_last_cmd(int i, t_fd fd, char *argv[], t_data *pipex, char *envp[]);
int		redirect_stdin_stdout(int new_stdin, int new_stdout, t_fd *fd, t_data *pipex);

/*Handle args functions*/
void	handle_argc_and_envp(int argc, char *envp[], t_data *pipe);
void	ft_split_paths(char const *s, char c, t_data *pipe);
void	ft_split_cmd(char const *s, char c, t_cmd *cmd);
void	test_cmd_permission(char **path, t_cmd *cmd);

/*Split utils*/
int		ft_count_args(const char *s, char c);
size_t	ft_arg_len(const char *str, char c);
char	*ft_trim_quotes(char *s1, const char *set);

/*Exit utils*/
void	ft_free_matrix(char **array, int j);
void	ft_handle_error(char *error_msg, t_data *pipex, t_fd *fd, int stage);
void	ft_close_pipes(int **pipe, int count);
void	wait_finish_pipe(t_fd *fd, t_data *pipex);

#endif
