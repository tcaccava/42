/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:15:58 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/27 15:49:38 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>
#include <string.h>

size_t	ft_strlcpy(char *dest, const char *src, size_t destsize)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = 0;
	while (src[src_len] != '\0')
	{
		src_len++;
	}
	if (destsize > 0)
	{
		while (src[i] != '\0' && i < destsize - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (src_len);
}
/*int main ()
{
	char src[] = "caparezza"; //lungo 9 caratteri
	char dest[6];           //buff piccolo, può contenere max 5 caratteri + '\0'
	size_t ris;             // variabile per salvare il valore restituito


	ris = ft_strlcpy(dest, src, sizeof(dest));

	printf("si parte da qua: %s\n",src);
	printf("si arriva qua: %s\n", dest);
	printf("quindi sono: %zu\n",ris);
}*/
