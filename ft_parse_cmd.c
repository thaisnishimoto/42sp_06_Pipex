/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:51:40 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/27 10:51:47 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"
#include "pipex.h"

size_t	ft_arg_len(char const *str, char c);
char	*ft_trim_quotes(char *s1, const char *set);

void	ft_split_paths(char const *s, char c, t_data *pipex)
{
	int		i;
	int		j;

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

	cmd->args = malloc((ft_count_args(s, ' ') + 1) * sizeof(char *));
	if (cmd->args == NULL)
		ft_handle_perror("malloc failed");
	i = 0;
	j = 0;
	while (j < ft_count_args(s, ' '))
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
		if ((ft_strlen(cmd->args[j]) > 2) && (cmd->args[j][0] == '\'' || cmd->args[j][0] == '\"'))
			cmd->args[j] = ft_trim_quotes(cmd->args[j], "\'\"");
		i = i + ft_arg_len(&s[i], c);
		j++;
	}
	cmd->args[j] = NULL;
}

char	*ft_add_path(char *path, char *cmd)
{
	size_t	path_len;
	size_t	cmd_len;
	char	*ptr;

	path_len = ft_strlen(path);
	cmd_len = ft_strlen(cmd);
	ptr = malloc((path_len +cmd_len + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	ft_memmove(ptr, path, path_len);
	ft_memmove(&ptr[path_len], cmd, cmd_len);
	ptr[path_len + cmd_len] = '\0';
	free(cmd);
	return (ptr);
}

char	*ft_trim_quotes(char *s1, const char *set)
{
	size_t	start;
	size_t	end;
	size_t	trim_len;
	char	*ptr;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	if (ft_strchr(set, s1[start]) && start < end)
		start++;
	if (ft_strrchr(set, s1[end - 1]) && end > start)
		end--;
	trim_len = end - start;
	ptr = malloc((trim_len + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	ft_strlcpy(ptr, &s1[start], trim_len + 1);
	free(s1);
	return (ptr);
}

int	ft_count_args(char const *s, char c)
{
	size_t	index;
	size_t	count;

	index = 0;
	count = 0;
	while (s[index])
	{
		while (s[index] == c)
			index++;
		if (s[index] && s[index] != c)
			count++;
		if (s[index] == '\'')
		{
			index++;
			while (s[index] && s[index] != '\'')
				index++;
			index++;
		}
		if (s[index] == '\"')
		{
			index++;
			while (s[index] && s[index] != '\"')
				index++;
			index++;
		}
		while (s[index] && s[index] != c)
			index++;
	}
	return (count);
}

size_t	ft_arg_len(char const *str, char c)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != c)
	{
		if (str[len] == '\'')
		{
			len++;
			while (str[len] && str[len] != '\'')
				len++;
			return (++len);
		}
		else if (str[len] == '\"')
		{
			len++;
			while (str[len] && str[len] != '\"')
				len++;
			return (++len);
		}
		len++;
	}
	return (len);
}
