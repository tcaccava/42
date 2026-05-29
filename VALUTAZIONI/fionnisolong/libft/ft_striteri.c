/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:16:40 by fionni            #+#    #+#             */
/*   Updated: 2024/12/30 15:47:33 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int	len;
	int	j;

	len = 0;
	j = 0;
	if (s != NULL && f != NULL)
	{
		len = ft_strlen(s);
		while (j < len)
		{
			(*f)(j, s);
			s++;
			j++;
		}
	}
}
