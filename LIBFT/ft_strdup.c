/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:01:14 by tcaccava          #+#    #+#             */
/*   Updated: 2024/12/11 16:47:45 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*d;
	int		i;
	int		j;

	i = 0;
	j = ft_strlen(s);
	d = (char *)malloc(sizeof(*d) * (j + 1));
	if (d == NULL)
		return (NULL);
	while (i < j)
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}
