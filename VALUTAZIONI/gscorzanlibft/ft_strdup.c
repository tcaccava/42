/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscorzon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 12:56:24 by gscorzon          #+#    #+#             */
/*   Updated: 2026/06/03 16:41:39 by gscorzon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		len;
	char	*dup;

	len = 0;
	while (s[len])
		len++;
	dup = malloc(len * sizeof(char) + 1);
	if (!dup)
		return (NULL);
	while (len >= 0)
	{
		dup[len] = s[len];
		len--;
	}
	return (dup);
}
