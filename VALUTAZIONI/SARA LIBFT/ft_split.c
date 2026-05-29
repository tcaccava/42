/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:55:13 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 15:55:24 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_str(char **matrix, int used)
{
	int	i;

	i = 0;
	while (i < used)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

static int	ft_wcount(const char *str, char c)
{
	int	count;
	int	in_word;
	int	i;

	count = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (str[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

static int	ft_sub_len(const char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	**fill_matrix(const char *s, char c, char **out, int words)
{
	int	i_sub;
	int	i_matrix;
	int	sub_len_var;

	i_sub = 0;
	i_matrix = 0;
	while (s[i_sub] && i_matrix < words)
	{
		while (s[i_sub] && s[i_sub] == c)
			i_sub++;
		if (s[i_sub])
		{
			sub_len_var = ft_sub_len(&s[i_sub], c);
			out[i_matrix] = ft_substr(s, i_sub, sub_len_var);
			if (!out[i_matrix])
			{
				free_str(out, i_matrix);
				return (NULL);
			}
			i_sub += sub_len_var;
			i_matrix++;
		}
	}
	out[i_matrix] = NULL;
	return (out);
}

char	**ft_split(const char *s, char c)
{
	char	**superstr;
	int		words;

	if (!s)
		return (NULL);
	words = ft_wcount(s, c);
	superstr = (char **)malloc(sizeof(char *) * (words + 1));
	if (!superstr)
		return (NULL);
	if (!fill_matrix(s, c, superstr, words))
		return (NULL);
	return (superstr);
}
