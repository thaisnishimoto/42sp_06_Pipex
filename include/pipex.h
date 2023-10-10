/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 23:07:52 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/10 17:22:11 by tmina-ni         ###   ########.fr       */
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
	int	pipe[2];
}	t_fd;

typedef struct s_fork
{
	pid_t	id1;
	pid_t	id2;
	int		wstatus1;
	int		wstatus2;
	int		exit_code;
}	t_fork;

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
	t_cmd	cmd;
}	t_data;

/*Pipe functions*/
void	open_file(int i, t_fd *fd, char *argv[], t_data *pipex);
void	exec_first_cmd(t_fd fd, char *argv[], t_data *pipex, char *envp[]);
void	exec_second_cmd(t_fd fd, char *argv[], t_data *pipex, char *envp[]);
int		redirect_stdin_stdout(int new_stdin, int new_stdout, t_fd *fd);

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
void	ft_close_pipe(t_fd *fd);
void	ft_close_stdfileno(void);
void	wait_finish_pipe(t_fd *fd, t_fork *process, t_data *pipex);

#endif
