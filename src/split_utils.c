/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmina-ni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 12:17:58 by tmina-ni          #+#    #+#             */
/*   Updated: 2023/10/10 18:17:48 by tmina-ni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	**ft_split_args(char const *s, char c)
{
	char	**array;
	int		i;
	int		j;

	array = malloc((ft_count_args(s, c) + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	i = 0;
	j = -1;
	while (++j < ft_count_args(s, c))
	{
		while (s[i] == c)
			i++;
		array[j] = malloc((ft_arg_len(&s[i], c) + 1) * sizeof(char));
		if (array[j] == NULL)
		{
			ft_free_matrix(array, j);
			return (NULL);
		}
		ft_strlcpy(array[j], &s[i], ft_arg_len(&s[i], c) + 1);
		array[j] = ft_trim_quotes(array[j], "\'\"");
		i = i + ft_arg_len(&s[i], c);
	}
	array[j] = NULL;
	return (array);
}

int	ft_count_args(char const *s, char c)
{
	int	index;
	int	count;

	index = 0;
	count = 0;
	while (s[index])
	{
		while (s[index] == c)
			index++;
		if (s[index] && s[index] != c)
			count++;
		if (s[index] == '\'' || s[index] == '\"')
			index += ft_preserve_quoted_substr(&s[index]);
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
		if (str[len] == '\'' || str[len] == '\"')
			len += ft_preserve_quoted_substr(&str[len]);
		len++;
	}
	return (len);
}

char	*ft_trim_quotes(char *str, const char *set)
{
	size_t	start;
	size_t	end;
	size_t	trim_len;
	char	*ptr;

	if (str == NULL || set == NULL)
		return (NULL);
	start = 0;
	end = ft_strlen(str);
	if (ft_strchr(set, str[start]) && start < end)
		start++;
	if (ft_strrchr(set, str[end - 1]) && end > start)
		end--;
	trim_len = end - start;
	ptr = malloc((trim_len + 1) * sizeof(char));
	if (ptr == NULL)
		return (NULL);
	ft_strlcpy(ptr, &str[start], trim_len + 1);
	free(str);
	return (ptr);
}

size_t	ft_preserve_quoted_substr(char const *str)
{
	size_t	len;

	len = 0;
	if (str[len] == '\'')
	{
		len++;
		while (str[len] && str[len] != '\'')
			len++;
		len++;
	}
	else if (str[len] == '\"')
	{
		len++;
		while (str[len] && str[len] != '\"')
			len++;
		len++;
	}
	return (len);
}
