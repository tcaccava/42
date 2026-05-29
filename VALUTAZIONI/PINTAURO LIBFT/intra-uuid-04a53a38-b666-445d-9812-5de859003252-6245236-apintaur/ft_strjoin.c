/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 04:19:12 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/15 22:26:34 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*new;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 && !s2)
		return (malloc(0));
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new = (char *) malloc (sizeof(char) * (s1_len + s2_len + 1));
	if (new == NULL)
		return (NULL);
	if (s1)
		ft_strlcpy(new, s1, s1_len + 1);
	if (s2)
		ft_strlcat(new, s2, s1_len + s2_len + 1);
	return (new);
}
