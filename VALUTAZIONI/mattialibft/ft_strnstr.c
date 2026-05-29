/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtozzi-b <mtozzi-b@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:18:50 by mtozzi-b          #+#    #+#             */
/*   Updated: 2025/01/15 22:30:07 by mtozzi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		i;
	size_t		j;
	const char	*h_bytes;
	const char	*n_bytes;

	i = 0;
	j = 0;
	h_bytes = (const char *)haystack;
	n_bytes = (const char *)needle;
	if (*n_bytes == '\0')
		return ((char *)h_bytes);
	if (len == 0)
		return (NULL);
	while (i < len && h_bytes[i] != '\0')
	{
		j = 0;
		while (i + j < len && h_bytes[i + j] == n_bytes[j] && n_bytes[j])
			j++;
		if (n_bytes[j] == '\0')
			return ((char *)&h_bytes[i]);
		i++;
	}
	return (NULL);
}
