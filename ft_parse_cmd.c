/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:51:40 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/09/20 18:39:21 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/libft.h"

size_t	ft_count_args(char const *s);
size_t	ft_arg_len(char const *str, char c);

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

char	**ft_split_cmd(char const *s, char c)
{
	size_t		i;
	size_t		j;
	char		**array;

	if (s == NULL)
		return (NULL);
	array = malloc((ft_count_args(s) + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (j < ft_count_args(s))
	{
		while (s[i] == c)
			i++;
		array[j] = malloc((ft_arg_len(&s[i], c) + 1) * sizeof(char));
		if (array[j] == NULL)
			return (NULL);
		ft_strlcpy(array[j], &s[i], ft_arg_len(&s[i], c) + 1);
		if ((ft_strlen(array[j]) > 2) && (array[j][0] == '\'' || array[j][0] == '\"'))
			array[j] = ft_trim_quotes(array[j], "\'\"");
		i = i + ft_arg_len(&s[i], c);
		j++;
	}
	array[j] = NULL;
	return (array);
}

size_t	ft_count_args(char const *s)
{
	size_t	index;
	size_t	count;

	index = 0;
	count = 0;
	while (s[index])
	{
		while (s[index] == ' ')
			index++;
		if (s[index] && s[index] != ' ')
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
		while (s[index] && s[index] != ' ')
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

//int	main(int argc, char *argv[])
//{
//	char	**array;
//	int	i;
//
//	array = ft_split_cmd(argv[1], ' ');
//	i = 0;
//	while(array[i])
//	{
//		ft_printf("array[%d]: %s\n", i, array[i]);
//		i++;
//	}
//	return (0);
//}
