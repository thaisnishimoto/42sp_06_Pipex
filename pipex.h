/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 23:07:52 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/26 16:19:06 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h> //pipe, dup, access
# include <fcntl.h> //open
# include <sys/types.h> //pid_t
# include <sys/wait.h> //wait
# include <errno.h> //errno
# include "./libft/libft.h"

typedef struct s_fd
{
        int     infile;
        int     outfile;
        int     pipe[2];
}       t_fd;

typedef struct s_fork
{
        pid_t   id1;
        pid_t   id2;
        int     wstatus1;
        int     wstatus2;
        int     exit_code;
}       t_fork;

typedef struct s_cmd
{
        char    *pathname;
        char     **args;
        int	args_count;
        int	exit_code;
}       t_cmd;

typedef struct s_data
{
        char     **path;
        int	path_count;
        int	cmd_count;
        t_cmd     cmd;
}       t_data;

void	ft_handle_args(int argc, char *argv[], char *envp[], t_data *pipe);

/*Parse functions*/
int	ft_count_args(const char *s, char c);
void	ft_split_paths(char const *s, char c, t_data *pipe);

/*Exit functions*/
void    ft_free_matrix(char **array, int j); 
void    ft_handle_perror(char *perror_msg); 
void    ft_handle_error(char *error_msg);
void    ft_close_pipe(t_fd *fd);
void    wait_finish_pipe(t_fd *fd, t_fork *process);


#endif
