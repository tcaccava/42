/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtozzi-b <mtozzi-b@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:53:44 by mtozzi-b          #+#    #+#             */
/*   Updated: 2025/01/14 14:00:38 by mtozzi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//dest è l'indirizzo della memoria che vuoi modificare.
//ch è il valore che vuoi scrivere nella memoria
//count è semplicemente il numero di byte che vuoi modificare a partire da ind-
//irizzo di dest.

void	*ft_memset(void *dest, int ch, size_t count)
{
	unsigned char	*bytes;

	bytes = (unsigned char *)dest;
	while (count)
	{
		count--;
		bytes[count] = ch;
	}
	return (dest);
}
