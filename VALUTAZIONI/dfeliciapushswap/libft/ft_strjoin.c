/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 15:18:46 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 15:17:37 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
/*#include <stdio.h>*/

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	int		i;
	int		n;

	i = 0;
	n = 0;
	if (!s1 || !s2)
		return (NULL);
	joined = malloc(sizeof(char) * ((ft_strlen(s1) + ft_strlen(s2)) + 1));
	if (!joined)
		return (NULL);
	while (s1[i] != '\0')
	{
		joined[i] = s1[i];
		i++;
	}
	while (s2[n] != '\0')
	{
		joined[i] = s2[n];
		i++;
		n++;
	}
	joined[i] = '\0';
	return (joined);
}
/*
int main()
{
		printf("%s\n", ft_strjoin("Ciao,", " come stai?");
}*/
