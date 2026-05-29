/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 00:17:02 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/11 14:25:41 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	unsigned char	us_c;

	us_c = (unsigned char)c;
	while (*str != '\0' && *str != us_c)
		str++;
	if (*str == us_c)
		return ((char *)str);
	return (NULL);
}
