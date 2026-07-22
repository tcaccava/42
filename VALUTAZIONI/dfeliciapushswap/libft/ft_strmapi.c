/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 16:20:53 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 15:29:49 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
/*#include <stdio.h>

int	ft_strlen(char const *s)
{
	int	i;
	
	i = 0;
	while(s[i] != '\0')
		i++;
	return (i);
}
*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*s2;

	i = 0;
	s2 = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!s2)
		return (NULL);
	while (s[i] != '\0')
	{
		s2[i] = f(i, s[i]);
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
/*
char	ft_toupper(unsigned int i, char b)
{
	(void)i;
	if (b >= 'a' && b<= 'z')
		return (b - 32);
	return (b);
}

int main()
{
	printf("%s\n", ft_strmapi("ciao come stai", ft_toupper));
}*/
