/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 04:36:54 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/16 22:17:44 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

static int	ft_isset(const char c, const char *set);

char	*ft_strtrim(const char *s, const char *set)
{
	char	*new;
	size_t	start;
	size_t	end;

	start = 0;
	end = 0;
	if (s == NULL || set == NULL)
		return (malloc(0));
	while (ft_isset(s[start], set))
		start++;
	if (s[start] == '\0')
		return (ft_calloc(1, sizeof(char)));
	while (s[end] != '\0')
		end++;
	while (end > start && ft_isset(s[end - 1], set))
		end--;
	new = (char *) malloc(sizeof(char) * (end - start + 1));
	if (new == NULL)
		return (NULL);
	ft_strlcpy(new, &(s[start]), end - start + 1);
	return (new);
}

static int	ft_isset(const char c, const char *set)
{
	size_t	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}
