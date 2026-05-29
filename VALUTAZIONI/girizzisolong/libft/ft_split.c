/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 21:00:33 by girizzi           #+#    #+#             */
/*   Updated: 2025/01/03 20:00:18 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_string_array(char **array, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static int	count_words(const char *str, char cut)
{
	int	word_count;
	int	in_word;
	int	i;

	word_count = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != cut && in_word == 0)
		{
			in_word = 1;
			word_count++;
		}
		else if (str[i] == cut)
			in_word = 0;
		i++;
	}
	return (word_count);
}

static int	sub_len(const char *str, char cut)
{
	int	length;

	length = 0;
	while (str[length] && str[length] != cut)
		length++;
	return (length);
}

static char	**fill(const char *str, char cut, char **array, int word_count)
{
	int	i;
	int	y;
	int	word_len;

	i = 0;
	y = 0;
	while (str[i] && y < word_count)
	{
		while (str[i] && str[i] == cut)
			i++;
		if (str[i])
		{
			word_len = sub_len(&str[i], cut);
			array[y] = ft_substr(str, i, word_len);
			if (!array[y])
			{
				free_string_array(array, y);
				return (NULL);
			}
			i += word_len;
			y++;
		}
	}
	array[y] = NULL;
	return (array);
}

char	**ft_split(const char *str, char cut)
{
	char	**result_array;
	int		word_count;

	if (!str)
		return (NULL);
	word_count = count_words(str, cut);
	result_array = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result_array)
		return (NULL);
	if (!fill(str, cut, result_array, word_count))
		return (NULL);
	return (result_array);
}

// int	main(void)
// {
// 	char	**split;

// 	split = ft_split("ciao amore ciao", ' ');
// 	printf("%s\n", split[0]);
// 	printf("%s\n", split[1]);
// 	printf("%s\n", split[2]);
// }