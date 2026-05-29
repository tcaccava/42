/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:52:12 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 15:52:15 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*s_str;

	i = 0;
	if (!str)
		return (NULL);
	s_str = (unsigned char *)str;
	while (i < n)
	{
		s_str[i] = c;
		i ++;
	}
	return (str);
}
