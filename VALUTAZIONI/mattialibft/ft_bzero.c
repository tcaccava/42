/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtozzi-b <mtozzi-b@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 00:17:06 by mtozzi-b          #+#    #+#             */
/*   Updated: 2025/01/14 22:23:10 by mtozzi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*bytes;
	size_t			i;

	bytes = (unsigned char *)s;
	i = 0;
	while (n)
	{
		bytes[i] = 0;
		i++;
		n--;
	}
}
//bytes lo utilizziamo per far si che s sia un unsigned char, ecco perchè il
//nome bytes, facciamo si che s valga 1 singolo byte.
//
//n è semplicemente quanto è grande l'index
//
//s è la destinazione (che ripeto, noi vogliamo in questo caso specifico
//lavorare per byte e quindi facciamo si che essa valga 1 byte inizializzandola
//in questo modo bytes = (unsigned char *)s; (char vale 1, int 4, ecc..)
