/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 20:19:32 by dd-alo            #+#    #+#             */
/*   Updated: 2026/07/28 19:04:19 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize > 0)
	{
		while (i < (dstsize - 1) && src[i] != '\0')
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}

//Implementa una funzione che copi una stringa src nel buffer dst
//senza superare la dimensione del buffer (size).

//La funzione deve copiare al massimo size - 1 caratteri,
//terminare la stringa con '\0' se size > 0 e
//restituire la lunghezza totale della stringa sorgente (src),
//indipendentemente da quanti caratteri siano stati copiati.