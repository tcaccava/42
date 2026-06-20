/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 16:50:48 by mpapale           #+#    #+#             */
/*   Updated: 2026/05/19 16:50:51 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		#include <string.h>

		size_t strlen(const char *s);

DESCRIPTION
		The strlen() function calculates the length of the string pointed to by
		s, excluding the terminating null byte ('\0').

RETURN VALUE
		The strlen() function returns the number of bytes in the string pointed
		to by s.
*/
#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	b;

	b = 0;
	while (*str)
	{
		b = b + 1;
		str++;
	}
	return (b);
}
