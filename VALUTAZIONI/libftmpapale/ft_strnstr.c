/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:47:08 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:47:08 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		char *
		strnstr(const char *big, const char *little, size_t len);

DESCRIPTION
		The strnstr() function locates the first occurrence of
		the null-terminated string little in the string big,
		where not more than len characters are searched.
		Characters that appear after a ‘\0’ character are not
		searched.  Since the strnstr() function is a FreeBSD specific API,
		it should only be used when portability is not a concern.
RETURN VALUES
		If little is an empty string,
		big is returned; if little occurs nowhere in big,
		NULL is returned; otherwise a pointer to the first
		character of the first occur‐
		rence of little is returned.
*/
#include "libft.h"

char	*ft_strnstr(const char *src, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (to_find[0] == 0)
		return ((char *)src);
	while (src[i] && i < len)
	{
		while (src[i + j] == to_find[j] && src[i + j] && i + j < len)
		{
			j++;
			if (to_find[j] == 0)
				return ((char *)src + i);
		}
		i++;
		j = 0;
	}
	return (0);
}
