/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:45:33 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:50:18 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	char *ft_substr(char const *s, unsigned int start,size_t len);

PARAMETERS
	s: The original string from which to create the substring.
	start: The starting index of the substring within ’s’.
	len: The maximum length of the substring.

DESCRIPTION
	Allocates memory (using malloc(3)) and returns a
	substring from the string ’s’.
	The substring starts at index ’start’ and has a
	maximum length of ’len’.

RETURN VALUES
	The substring. NULL if the allocation fails.

*/
#include "libft.h"

char	*ft_substr(char const *src, unsigned int start, size_t len)
{
	char	*substr;
	size_t	src_len;

	if (!src)
		return (NULL);
	src_len = ft_strlen(src);
	if (start >= src_len)
		return (ft_strdup(""));
	if (len > src_len - start)
		len = src_len - start;
	substr = malloc(sizeof(*substr) * (len + 1));
	if (!substr)
		return (NULL);
	ft_strlcpy(substr, src + start, len + 1);
	return (substr);
}
