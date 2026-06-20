/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:45:41 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:45:41 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		char *strrchr(const char *s, int c);

DESCRIPTION
		The strrchr() function returns a pointer to the last occurrence of the
		character c in the string s.

RETURN VALUE
		The strchr() and strrchr() functions return a pointer to the matched
		character or NULL if the character is not
		found.   The  terminating null byte is considered part of the string,
		so that if c is specified as '\0', these
		functions return a pointer to the terminator.
*/
#include "libft.h"

char	*ft_strrchr(const char *str, int to_find)
{
	size_t	i;
	char	c;

	c = (char)to_find;
	i = ft_strlen(str);
	while (1)
	{
		if (str[i] == c)
			return ((char *)str + i);
		if (i == 0)
			break ;
		i--;
	}
	return (NULL);
}
