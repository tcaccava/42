/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 19:08:45 by mgentile          #+#    #+#             */
/*   Updated: 2026/05/22 17:41:34 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>
#include <stdio.h>

void	ft_bzero(void *s, size_t n)
{
	char	*x;
	size_t	i;

	x = (char *)s;
	i = 0;
	while (i < n)
	{
		x[i] = 0;
		i++;
	}
}

/*int main()
{
	int buf[] = {1, 6, 8, 9};
	ft_bzero(buf, 16);
	int i = 0;

	while (i < 4)
	{
		printf("%d ", buf[i]);
		i++;
	}
	return (0);
}*/
