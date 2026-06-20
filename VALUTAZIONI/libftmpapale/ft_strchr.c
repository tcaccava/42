/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:45:05 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:45:05 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		char *strchr(const char *s, int c);

DESCRIPTION
		The strchr() function returns a pointer to the first occurrence of
		the character c in the string s.

		Here "character" means "byte"; these functions do not work with wide
		or multibyte characters.

RETURN VALUE
		The  strchr() and strrchr() functions return a pointer to the matched
		character or NULL if the character is not found.  The terminating null
		byte is considered part
		of the string, so that if c is specified as '\0', these functions
		return a pointer to the terminator.
*/
#include "libft.h"

char	*ft_strchr(const char *str, int to_find)
{
	while (*str)
	{
		if (*str == (char)to_find)
			return ((char *)str);
		str++;
	}
	if (*str == (char)to_find)
		return ((char *)str);
	return (NULL);
}
