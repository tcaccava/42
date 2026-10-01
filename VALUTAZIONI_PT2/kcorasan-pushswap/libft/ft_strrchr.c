/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:17:27 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/27 15:36:15 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>
#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	const char	*ult;

	ult = NULL;
	while (*s)
	{
		if (*s == (char)c)
			ult = s;
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return ((char *)ult);
}

/*int main()
{
	char *k;
	char l [] = "gianfilippo";
	k = ft_strrchr(l, 'i');
	printf("se funziona stampi: %s\n", k);
	printf("funziona or: %s\n", strrchr(l, 'i'));

}*/
