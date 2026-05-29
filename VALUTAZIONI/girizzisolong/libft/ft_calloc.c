/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 14:30:13 by girizzi           #+#    #+#             */
/*   Updated: 2025/01/04 19:27:08 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t dim)
{
	void	*str;

	if (dim != 0 && num > SIZE_MAX / dim)
	{
		return (NULL);
	}
	str = malloc(num * dim);
	if (!str)
		return (NULL);
	ft_bzero(str, num * dim);
	return (str);
}
