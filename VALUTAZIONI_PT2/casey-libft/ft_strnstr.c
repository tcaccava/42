/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 18:20:40 by keiestre          #+#    #+#             */
/*   Updated: 2026/09/23 18:20:41 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	l;

	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	while (i < len && big[i] != '\0')
	{
		l = 0;
		if (big[i] == little[l])
		{
			while (i < len && big[i] != '\0' && big[i] == little[l])
			{
				i++;
				l++;
			}
			if (little[l] == '\0')
				return ((char *)&big[i - l]);
			i = i - l;
		}
		i++;
	}
	return (NULL);
}
