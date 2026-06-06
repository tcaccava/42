/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 18:35:22 by ggiument          #+#    #+#             */
/*   Updated: 2026/05/26 18:50:09 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*array;
	size_t	i;
	size_t	o;

	i = 0;
	o = 0;
	if (!s1 || !s2)
		return (NULL);
	array = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!array)
		return (NULL);
	while (s1[i])
	{
		array[i] = s1[i];
		i++;
	}
	while (s2[o])
	{
		array[i] = s2[o];
		i++;
		o++;
	}
	array[i] = '\0';
	return (array);
}
