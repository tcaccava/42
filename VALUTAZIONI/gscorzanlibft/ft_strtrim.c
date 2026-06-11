/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscorzon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 14:54:12 by gscorzon          #+#    #+#             */
/*   Updated: 2026/06/04 19:09:21 by gscorzon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char const c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	st;
	int	en;

	if (!s1 || !set)
		return (NULL);
	st = 0;
	en = ft_strlen(s1);
	while (is_set(s1[st], set))
		st++;
	if (st == en)
		return (ft_strdup(""));
	while (is_set(s1[en - 1], set))
		en--;
	return (ft_substr(s1, st, en - st));
}
