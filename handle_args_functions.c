/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:51:40 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/28 18:21:53 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"
#include "pipex.h"

void	handle_argc_and_envp(int argc, char *envp[], t_data *pipex)
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
	ft_split_paths(envp[i] + 5, ':', pipex);
}

void	ft_split_paths(char const *s, char c, t_data *pipex)
{
	int		i;
	int		j;

	pipex->path_count = ft_count_args(s, ':');
	pipex->path = malloc((pipex->path_count + 1) * sizeof(char *));
	if (pipex->path == NULL)
		ft_handle_perror("malloc failed");
	i = 0;
	j = 0;
	while (j < pipex->path_count)
	{
		while (s[i] == c)
			i++;
		pipex->path[j] = malloc((ft_arg_len(&s[i], c) + 2) * sizeof(char));
		if (pipex->path[j] == NULL)
		{
			ft_free_matrix(pipex->path, --j);
			ft_handle_perror("malloc failed");
		}
		ft_strlcpy(pipex->path[j], &s[i], ft_arg_len(&s[i], c) + 1);
		ft_strlcat(pipex->path[j], "/", ft_arg_len(&s[i], c) + 2);
		i = i + ft_arg_len(&s[i], c);
		j++;
	}
	pipex->path[j] = NULL;
}

void	ft_split_cmd(char const *s, char c, t_cmd *cmd)
{
	int		i;
	int		j;

	cmd->args_count = ft_count_args(s, ' ');
	cmd->args = malloc((cmd->args_count + 1) * sizeof(char *));
	if (cmd->args == NULL)
		ft_handle_perror("malloc failed");
	i = 0;
	j = 0;
	while (j < cmd->args_count)
	{
		while (s[i] == c)
			i++;
		cmd->args[j] = malloc((ft_arg_len(&s[i], c) + 1) * sizeof(char));
		if (cmd->args[j] == NULL)
		{
			ft_free_matrix(cmd->args, --j);
			ft_handle_perror("malloc failed");
		}
		ft_strlcpy(cmd->args[j], &s[i], ft_arg_len(&s[i], c) + 1);
		cmd->args[j] = ft_trim_quotes(cmd->args[j], "\'\"");
		i = i + ft_arg_len(&s[i], c);
		j++;
	}
	cmd->args[j] = NULL;
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
			break ;
	}
}
