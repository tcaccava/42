/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:08:01 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/26 12:22:52 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*#include <stdio.h>*/
#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*a;
	char	*b;

	i = 0;
	if (!dest && !src)
		return (NULL);
	a = (char *)src;
	b = (char *)dest;
	while (i < n)
	{
		b[i] = a[i];
		i++;
	}
	return (dest);
}
/*
int	main()
{
	char dest[9] = "AAAoooooo";
	char src[4] = "Ciao";
	ft_memcpy(dest, src, 3);
	printf("%s\n", dest);
	return (0);
}*/
