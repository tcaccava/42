/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:39:35 by keiestre          #+#    #+#             */
/*   Updated: 2026/07/06 20:12:56 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		n;
	int		j;
	char	*dest;

	n = 0;
	j = 0;
	if (s == NULL)
		return (NULL);
	while (s[n] != '\0')
		n++;
	dest = (char *)malloc((n + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (j < n)
	{
		dest[j] = s[j];
		j++;
	}
	dest[n] = '\0';
	return (dest);
}
