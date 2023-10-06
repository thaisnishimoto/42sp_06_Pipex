/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_functions_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:06:44 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/06 11:17:17 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	ft_split_paths(char const *s, char c, t_data *pipex)
{
	int		i;
	int		j;

	pipex->path_count = ft_count_args(s, ':');
	pipex->path = malloc((pipex->path_count + 1) * sizeof(char *));
	if (pipex->path == NULL)
		ft_handle_error("malloc failed", NULL, NULL, 0);
	i = 0;
	j = 0;
	while (j < pipex->path_count)
	{
		while (s[i] == c)
			i++;
		pipex->path[j] = malloc((ft_arg_len(&s[i], c) + 2) * sizeof(char));
		if (pipex->path[j] == NULL)
		{
			ft_free_matrix(pipex->path, j);
			ft_handle_error("malloc failed", NULL, NULL, 0);
		}
		ft_strlcpy(pipex->path[j], &s[i], ft_arg_len(&s[i], c) + 1);
		ft_strlcat(pipex->path[j], "/", ft_arg_len(&s[i], c) + 2);
		i = i + ft_arg_len(&s[i], c);
		j++;
	}
	pipex->path[j] = NULL;
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
