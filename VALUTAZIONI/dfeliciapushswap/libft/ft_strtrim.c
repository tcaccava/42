/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:05:56 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 18:32:50 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
/*#include <stdio.h>*/

int	trimcheck(char s1, char const *set)
{
	int	n;
	int	check;

	n = 0;
	check = 0;
	while (set[n] != '\0')
	{
		if (s1 == set[n])
			check = 1;
		n++;
	}
	return (check);
}
/*
int	ft_strlen(const char *s1)
{
	int	i;

	i = 0;
	while (s1[i] != '\0')
		i++;
	return (i);
}*/

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		n;
	int		j;
	char	*trim;

	j = 0;
	i = 0;
	if (!s1 || !set)
		return (NULL);
	n = ft_strlen(s1);
	while (trimcheck(s1[i], set) != 0)
		i++;
	while (n > 0 && trimcheck(s1[n - 1], set) != 0)
		n--;
	if (n > i)
		trim = malloc(sizeof(char) * ((n - i) + 1));
	else
		trim = malloc(1);
	if (!trim)
		return (NULL);
	while (i < n)
		trim[j++] = s1[i++];
	trim[j] = '\0';
	return (trim);
}
/*
int	main()
{
	printf("%s\n", ft_strtrim("xxdgdxxCiao, Comexdggdg", ""));
}*/
