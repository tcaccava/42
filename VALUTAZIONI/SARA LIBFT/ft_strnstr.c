/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:00:47 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 16:00:50 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if ((!str || !to_find) && len == 0)
		return (NULL);
	if (to_find[0] == '0' || to_find == str)
		return ((char *) str);
	while (str[i] != '\0')
	{
		j = 0;
		while ((str[i + j] == to_find[j] && (i + j) < len))
		{
			if (str[i + j] == 0 && to_find[j] == '\0')
				return ((char *)(str + i));
			j++;
		}
		if (to_find[j] == '\0')
			return ((char *)(str + i));
		i++;
	}
	return (0);
}
