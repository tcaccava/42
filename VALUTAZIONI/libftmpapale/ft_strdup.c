/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:26 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:26 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		char *strdup(const char *s);
DESCRIPTION
		The strdup() function returns a pointer to a new string
		which is a duplicate of the string s.  Memory for the
		new string is obtained with malloc(3),
		and can be freed with free(3).
RETURN VALUE
		On success, the strdup() function returns a pointer to
		the duplicated string.
		It returns NULL if insufficient memory was available,
		with errno set to indicate the error.
ERRORS
		ENOMEM Insufficient memory available to allocate duplicate
		string.
*/
#include "libft.h"

char	*ft_strdup(const char *src)
{
	size_t	src_len;
	char	*array;

	src_len = ft_strlen((char *)src);
	array = (char *)malloc(src_len + 1);
	if (!array)
		return (NULL);
	ft_strlcpy(array, src, src_len + 1);
	return ((char *)array);
}
