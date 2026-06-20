/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:45:38 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 16:13:08 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	char *ft_strtrim(char const *s1, char const *set);

PARAMETERS
	s1: The string to be trimmed.
	set: The string containing the set of characters to be removed.

DESCRIPTION
	Allocates memory (using malloc(3)) and returns a
	copy of ’s1’ with characters from ’set’ removed
	from the beginning and the end.

RETURN VALUES
	The trimmed string. NULL if the allocation fails.
*/
#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int				start;
	int				end;
	unsigned char	*str;
	unsigned char	*cset;
	char			*trimmed;

	str = (unsigned char *)s1;
	cset = (unsigned char *)set;
	start = 0;
	if (!s1 || !set)
		return (NULL);
	while (str[start] && ft_strchr((char *)cset, str[start]))
		start++;
	end = ft_strlen((char *)str) - 1;
	while (end >= start && ft_strchr((char *)cset, str[end]))
		end--;
	trimmed = malloc((end - start + 1) + 1);
	if (!trimmed)
		return (NULL);
	ft_strlcpy(trimmed, (char *)str + start, end - start + 2);
	return (trimmed);
}
