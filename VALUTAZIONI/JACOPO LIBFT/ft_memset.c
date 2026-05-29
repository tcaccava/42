/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 01:36:52 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/08 20:12:48 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int filler, size_t size)
{
	char	*copy;
	char	c_filler;

	copy = (char *)dest;
	c_filler = (char)filler;
	while (size > 0)
	{
		*copy = c_filler;
		copy++;
		size--;
	}
	return (dest);
}
