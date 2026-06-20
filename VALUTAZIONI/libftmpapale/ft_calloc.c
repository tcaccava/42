/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 16:52:38 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 16:52:44 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		void *calloc(size_t n, size_t size);

DESCRIPTION

   calloc()
		The calloc() function allocates memory for an array of n elements of size
		bytes each and returns a pointer to the allocated memory.  The memory is
		set to zero.
		If n or size is 0, then calloc() returns a unique pointer value that can
		later be successfully passed to free().

		If  the multiplication of n and size would result in integer overflow,
		then calloc() returns an error.  By contrast, an integer overflow would
		not be detected in the following call to malloc(), with the result that
		an incorrectly sized block of memory would be allocated:

			malloc(n * size);
*/
#include "libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	void	*array;

	array = (void *)malloc(n * size);
	if (!array)
		return (NULL);
	ft_bzero(array, size * n);
	return ((void *)array);
}
