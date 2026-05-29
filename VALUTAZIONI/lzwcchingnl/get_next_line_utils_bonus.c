/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzecchin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:29:36 by lzecchin          #+#    #+#             */
/*   Updated: 2025/04/28 18:29:39 by lzecchin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	if (c == '\0')
		return ((char *)(s + ft_strlen_0(s)));
	while (*s)
	{
		if (*s == (unsigned char)c)
			return ((char *) s);
		s++;
	}
	return (NULL);
}

size_t	ft_strlen_0(const char *s)
{
	int	len;

	len = 0;
	if (s)
	{
		while (s[len])
			len++;
	}
	return (len);
}

void	*ft_memcpy_0term(void *dst, const void *src, size_t n)
{
	const unsigned char	*s1;
	unsigned char		*s2;

	s1 = (const unsigned char *) src;
	s2 = (unsigned char *) dst;
	s2[n] = '\0';
	while (n > 0)
	{
		s2[n - 1] = s1[n - 1];
		n--;
	}
	return ((void *)s2);
}

char	*ft_concatenate(char **saved, char *buffer, int n)
{
	int		len;
	char	*conc;

	if (n == 0)
		return (*saved);
	len = ft_strlen_0(*saved);
	conc = malloc (len + n + 1);
	if (!conc)
		return (NULL);
	if (*saved)
		ft_memcpy_0term(conc, *saved, len);
	ft_memcpy_0term(conc + len, buffer, n);
	free(*saved);
	*saved = conc;
	return (conc);
}

char	*ft_getline_update_saved(char **saved, int n)
{
	char	*line;
	char	*end_line_pos;
	char	*new_saved;

	if (n == 0)
	{
		line = *saved;
		*saved = NULL;
		return (line);
	}
	end_line_pos = ft_strchr(*saved, '\n');
	line = malloc (end_line_pos - *saved + 2);
	if (!line)
		return (NULL);
	ft_memcpy_0term(line, *saved, end_line_pos - *saved + 1);
	new_saved = malloc(ft_strlen_0(end_line_pos + 1) + 1);
	if (!new_saved)
	{
		free(line);
		return (NULL);
	}
	ft_memcpy_0term(new_saved, end_line_pos + 1, ft_strlen_0(end_line_pos + 1));
	free(*saved);
	*saved = new_saved;
	return (line);
}
