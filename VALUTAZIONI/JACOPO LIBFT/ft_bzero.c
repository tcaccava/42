/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 12:08:07 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/08 20:11:34 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *src, size_t size)
{
	char	*c_src;
	size_t	i;

	i = 0;
	c_src = (char *)src;
	while (i < size)
	{
		c_src[i] = '\0';
		i++;
	}
}
