/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:58:57 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/13 12:37:48 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	char	*tmp;
	size_t	i;

	i = 0;
	tmp = NULL;
	while (s[i] != '\0')
	{
		if (s[i] == (unsigned char)c)
			tmp = &(((char *)(s))[i]);
		i++;
	}
	if (c == '\0')
	{
		return (&(((char *)(s))[i]));
	}
	return (tmp);
}
