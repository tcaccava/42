/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:17:09 by fionni            #+#    #+#             */
/*   Updated: 2024/12/31 14:40:54 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	y;

	i = 0;
	if (to_find[0] == '\0')
		return ((char *) str);
	while (str[i] != '\0')
	{
		y = 0;
		while (str[i + y] == to_find[y] && (i + y) < len)
		{
			if (str[i + y] == '\0' && to_find[y] == '\0')
				return ((char *)&str[i]);
			y++;
		}
		if (to_find[y] == '\0')
			return ((char *)str + i);
		i++;
	}
	return (0);
}
