/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 22:16:19 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/16 23:03:55 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strnstr(const char *str, const char *to_find, size_t n)
{
	size_t	i;
	size_t	k;

	i = 0;
	if (!str || !to_find)
		return (NULL);
	if (*to_find == '\0')
		return (((char *)str));
	while (str[i] != '\0')
	{
		k = 0;
		while (str[i + k] == to_find[k] && (i + k) < n)
		{
			if (to_find[k + 1] == '\0' || (k + i) == n)
				return (&(((char *)(str))[i]));
			k++;
		}
		i++;
	}
	return (NULL);
}
