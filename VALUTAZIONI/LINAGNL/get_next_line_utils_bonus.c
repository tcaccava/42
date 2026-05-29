/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:25:12 by ligharbi          #+#    #+#             */
/*   Updated: 2025/02/01 05:25:16 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	*my_bzero(void *s, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n > 0)
	{
		*p = '\0';
		n --;
		p ++;
	}
	return (s);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	void	*p;

	if (size != 0 && nmemb >= ((size_t) - 1) / size)
		return (NULL);
	total = nmemb * size;
	p = malloc (total);
	if (p == NULL)
		return (NULL);
	my_bzero(p, total);
	return (p);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	int		k;
	char	*p;

	k = 0;
	j = ft_strlen(s1);
	i = ft_strlen(s2) +j;
	p = malloc (i + 1);
	if (p == NULL)
		return (NULL);
	while (k < j)
	{
		p[k] = s1[k];
		k ++;
	}
	j = 0;
	while (k < i)
	{
		p[k] = s2[j];
		k ++;
		j ++;
	}
	p[k] = '\0';
	return (p);
}

int	ft_strlen(const char *s)
{
	int	n;

	n = 0;
	while (s[n] != '\0')
	{
		n ++;
	}
	return (n);
}

char	*ft_strchr(const char *s, int c)
{
	char	*r;

	r = (char *)s;
	while (*r != '\0')
	{
		if (*r == (char)c)
			return (r);
		r++;
	}
	if (c == '\0')
		return (r);
	return (NULL);
}
