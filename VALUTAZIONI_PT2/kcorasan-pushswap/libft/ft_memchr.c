/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:32:18 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/27 15:46:54 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*str;

	str = (const unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (str[i] == (unsigned char)c)
			return ((void *)&str[i]);
		i++;
	}
	return (NULL);
}

/*int	main(void)
{
		char prova[] = "sono una sirena";
		char *ris;
		ris = ft_memchr(prova, 'a', strlen(prova));
		//Qua gli sto dicendo di cercare a in n byte (non li ho contati tutti
ok?) printf("ho trovato la %c qui -> %ld\n", *ris, ris - prova); //ld long int,
		solo d avrebbe un comport sbagliato
}*/
