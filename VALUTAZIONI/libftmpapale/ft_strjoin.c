/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:28 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:47:15 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	char *ft_strjoin(char const *s1, char const *s2);

PARAMETERS
	s1: The prefix string.
	s2: The suffix string.

DESCRIPTION
	Allocates memory (using malloc(3)) and returns a
	new string, which is the result of concatenating
	’s1’ and ’s2’.

RETURN VALUES
	The new string. NULL if the allocation fails.
*/
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joinstr;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	joinstr = malloc(s1_len + s2_len + 1);
	if (!joinstr)
		return (NULL);
	ft_strlcpy(joinstr, s1, s1_len + 1);
	ft_strlcat(joinstr, s2, s1_len + s2_len + 1);
	return (joinstr);
}
