/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 18:41:21 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 19:52:52 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ss1;
	size_t			i;

	ss1 = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ss1[i] = c;
		i++;
	}
	return (s);
}

/*int main()
{
	char *ss1 = "le polpette sono buone";
	char *ss2 = "le polipette";
	size_t result;

	result = ft_memcmp(ss1, ss2, 22);
	printf("%zu\n", result);
}*/
