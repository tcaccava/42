/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 09:17:53 by lonulli           #+#    #+#             */
/*   Updated: 2024/12/23 09:17:54 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

/*Function to look for a certain char inside a string.
In my case we'll use it to search a '\n'*/

int	ft_strchr(char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}

/*Calculate string lenght*/

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

/*Duplicate a given string using malloc.*/

char	*ft_strdup(char *str)
{
	int		i;
	char	*new;
	int		len;

	i = 0;
	if (!str)
	{
		new = malloc(sizeof(char));
		*new = '\0';
		return (new);
	}
	len = ft_strlen(str);
	new = malloc(sizeof(char) * len + 1);
	if (!new)
		return (NULL);
	while (str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

/*Join 2 strings togheter in a new one, using malloc*/

char	*ft_strjoin(char *s1, char *s2)
{
	char	*result;
	size_t	s1len;
	size_t	s2len;
	char	*ret;

	if (!s1)
		return (ft_strdup(s2));
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	result = (char *)malloc(sizeof(char) * (s1len + s2len + 1));
	if (!result)
		return (NULL);
	ret = result;
	while (*s1)
		*result++ = *s1++;
	while (*s2)
		*result++ = *s2++;
	*result = '\0';
	return (ret);
}

char	*safe_malloc(void)
{
	char	*line;

	line = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!line)
		return (NULL);
	return (line);
}
