/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:49:28 by lvovlas           #+#    #+#             */
/*   Updated: 2026/07/16 12:18:49 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	last;

	i = 0;
	last = -1;
	while (s[i])
	{
		if ((unsigned char)s[i] == (unsigned char)c)
			last = i;
		i++;
	}
	if (c == '\0')
		last = i;
	if (last == -1)
		return (NULL);
	return ((char *)(s + last));
}
