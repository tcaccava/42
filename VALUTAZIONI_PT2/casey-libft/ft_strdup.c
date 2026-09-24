/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 18:19:42 by keiestre          #+#    #+#             */
/*   Updated: 2026/09/23 18:19:43 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	int		n;
	int		j;
	char	*dest;

	n = 0;
	j = 0;
	while (str[n] != '\0')
		n++;
	dest = (char *)malloc((n + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (j < n)
	{
		dest[j] = str[j];
		j++;
	}
	dest[n] = '\0';
	return (dest);
}
