/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:00 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:00 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		int memcmp(size_t n;
		const void s1[n], const void s2[n], size_t n);
DESCRIPTION
		The memcmp() function compares the first n bytes
		(each interpreted as unsigned char)
		of the memory areas s1 and s2.
RETURN VALUE
		The  memcmp() function returns an integer less
		than, equal to, or greater than zero
		if the first n bytes of s1 is found,
		respectively, to be less than, to match,
		or be greater than the first n bytes of s2.

		For a nonzero return value,
		the sign is determined by the sign of the difference
		between the first pair of bytes (inter‐
		preted as unsigned char) that differ in s1 and s2.

		If n is zero, the return value is zero.
*/
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*ptr1;
	const unsigned char	*ptr2;

	i = 0;
	ptr1 = (const unsigned char *)s1;
	ptr2 = (const unsigned char *)s2;
	while (i < n)
	{
		if (ptr1[i] != ptr2[i])
			return (ptr1[i] - ptr2[i]);
		i++;
	}
	return (0);
}
