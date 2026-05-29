/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 21:35:00 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/13 09:35:33 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Copia una stringa in un buffer con terminazione '\0'.
 * @param dest: Buffer di destinazione.
 * @param src: Stringa sorgente da copiare.
 * @param size: Dimensione del buffer (incluso '\0').
 * @return Lunghezza di `src` (escluso '\0').
 *
 * Copia fino a `size - 1` caratteri in `dest` e aggiunge 
 * sempre '\0' se `size > 0`. Restituisce la lunghezza 
 * completa di `src`, utile per verificare il troncamento.
 */
size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (size == 0)
		return (ft_strlen(src));
	i = 0;
	while (i < size - 1 && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	while (src[i] != '\0')
		i++;
	return (i);
}
