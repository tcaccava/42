/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:15:09 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/27 15:49:09 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>
#include <string.h>

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

/*int main()
{
	const char  *str = "Voglio moRiRe, dio santo";
	char		*res_originale;
		char		*res_mia;

		printf("1: Carattere ('R') %c\n", 'R');

		res_originale = strchr(str, 'R');
		res_mia = ft_strchr(str, 'R');

		printf("ori: %s , mia: %s\n\n", res_originale, res_mia);


	printf("2: Carattere ('Z') %c\n", 'Z');

		res_originale = strchr(str, 'Z');
		res_mia = ft_strchr(str, 'Z');

		printf("ori: %s , mia: %p\n", res_originale, res_mia);


	printf("3: Carattere ('\\0') %c\n", '\0');

		res_originale = strchr(str, '\0');
		res_mia = ft_strchr(str, '\0');

		printf("ori: %s , mia: %s\n\n", res_originale, res_mia);

}*/
