/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 15:44:15 by ggiument          #+#    #+#             */
/*   Updated: 2026/06/04 20:42:14 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	o;

	i = 0;
	o = 0;
	if (!(*little))
		return ((char *) big);
	while (i < len && big[i] && little[o])
	{
		if (big[i] == little[o])
		{
			i++;
			o++;
		}
		else
		{
			i = i - o + 1;
			o = 0;
		}
	}
	if (o == 0 || little[o] != '\0')
		return (NULL);
	return ((char *) & big[i - o]);
}
