/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:04:50 by lvovlas           #+#    #+#             */
/*   Updated: 2026/06/05 13:54:03 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*temp_s1;
	const unsigned char	*temp_s2;

	temp_s1 = (const unsigned char *) s1;
	temp_s2 = (const unsigned char *) s2;
	while (n--)
	{
		if (*temp_s1 != *temp_s2)
			return ((int) *temp_s1 - (int) *temp_s2);
		temp_s1++;
		temp_s2++;
	}
	return (0);
}
