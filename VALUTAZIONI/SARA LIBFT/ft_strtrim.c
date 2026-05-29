/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:01:33 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 16:01:35 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set ++;
	}
	return (0);
}

static char	*trim_cpy(char *dest, const char *src, int start, int end)
{
	int	i;

	i = 0;
	while (start <= end)
	{
		dest[i] = src[start];
		i ++;
		start ++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*s_trim;
	int		i_s;
	int		i_e;
	int		size;

	i_s = 0;
	if (!s1 || !set)
		return (NULL);
	i_e = ft_strlen(s1) - 1;
	while (s1[i_s] != '\0' && in_set(s1[i_s], set) == 1)
	{
		i_s ++;
	}
	while (i_e > i_s && in_set(s1[i_e], set) == 1)
	{
		i_e --;
	}
	size = i_e - i_s;
	s_trim = (char *) malloc(sizeof(char) * (size + 2));
	if (!s_trim)
		return (NULL);
	return (trim_cpy(s_trim, s1, i_s, i_e));
}
