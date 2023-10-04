/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_args_functions_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:51:40 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/04 17:04:01 by tmina-ni         ###   ########.fr       */
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
			ft_free_matrix(pipex->path, --j);
			ft_handle_error("malloc failed", NULL, NULL, 0);
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
		ft_handle_error("malloc failed", NULL, NULL, 0);
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
			ft_handle_error("malloc failed", NULL, NULL, 0);
		}
		ft_strlcpy(cmd->args[j], &s[i], ft_arg_len(&s[i], c) + 1);
		cmd->args[j] = ft_trim_quotes(cmd->args[j], "\'\"");
		i = i + ft_arg_len(&s[i], c);
		j++;
	}
	cmd->args[j] = NULL;
}
