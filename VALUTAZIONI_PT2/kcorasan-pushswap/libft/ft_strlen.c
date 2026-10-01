/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:16:37 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/27 15:36:11 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stddef.h>
#include <stdio.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

/*int main()
{
	char d [] = "abrastoca";
	printf("devo provare %s\n", d);
	int n = ft_strlen(d);
	printf("non credo di esserci %zu", n);
	return (0);
}*/
