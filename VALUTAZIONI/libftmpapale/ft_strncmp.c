/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:47:06 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:47:06 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		#include <string.h>

		int strcmp(const char *s1, const char *s2);
		int strncmp(const char s1[], const char s2[], size_t n);

DESCRIPTION
		The strcmp() function compares the two strings s1 and s2.
		The locale is not taken into account (for a locale-
		aware comparison, see strcoll(3)).  The comparison is done
		using unsigned characters.

		strcmp() returns an integer indicating the result of the
		comparison, as follows:

		•  0, if the s1 and s2 are equal;

		•  a negative value if s1 is less than s2;

		•  a positive value if s1 is greater than s2.

		The strncmp() function is similar, except it compares only
		the first (at most) n bytes of s1 and s2.

RETURN VALUE
		The strcmp() and strncmp() functions return an integer less
		than, equal to, or greater than zero if s1 (or the
		first n bytes thereof) is found, respectively, to be less
		than, to match, or be greater than s2.
*/
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
