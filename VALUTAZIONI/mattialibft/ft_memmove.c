/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtozzi-b <mtozzi-b@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 22:45:31 by mtozzi-b          #+#    #+#             */
/*   Updated: 2025/01/18 18:03:13 by mtozzi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t				i;
	unsigned char		*bytes_dst;
	const unsigned char	*bytes_src;

	if (dst == NULL || src == NULL)
		return (NULL);
	bytes_dst = (unsigned char *)dst;
	bytes_src = (unsigned char *)src;
	if (bytes_dst < bytes_src)
	{
		i = 0;
		while (len--)
		{
			bytes_dst[i] = bytes_src[i];
			i++;
		}
	}
	else
	{
		i = len;
		while (i--)
			bytes_dst[i] = bytes_src[i];
	}
	return (dst);
}
//IMPORTANTISSIMO
//si fa bytes_dst[i - 1] ecc, non per chissà che motivo arcano, ma perchè quand
//l'user va a mette l'input e per esempio mette che len è 3 noi non partiamo da
//3 nell'index ma da 2, perchè è 0, 1, 2, (3) capito? si.
//ti lascio di seguito come l'hai fatta inizialmente che era decisamente meglio
//#include "libft.h"

//void	*ft_memmove(void *dst, const void *src, size_t len)
//{
//	size_t	i;
//	unsigned char	*bytes_dst;
//	const unsigned char	*bytes_src;
//
//	i = 0;
//	bytes_dst = (unsigned char *)dst;
//	bytes_src = (unsigned char *)src;
//	if (bytes_dst < bytes_src)
//	{
//		while (len)
//		{
//			bytes_dst[i] = bytes_src[i];
//			i++;
//			len--;
//		}
//	}
//	else
//	{
//		i = len;
//		while (i)
//		{
//			bytes_dst[i - 1] = bytes_src[i - 1];
//			i--;
//		}
//	}
//	return (dst);
//}
