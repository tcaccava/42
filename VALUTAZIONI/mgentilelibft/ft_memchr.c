/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:59:26 by mgentile          #+#    #+#             */
/*   Updated: 2026/05/30 20:00:51 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ss;
	size_t			i;

	ss = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ss[i] != (unsigned char)c)
			i++;
		else
			return (ss + i);
	}
	return (NULL);
}

/*int	main(void)
{
	char	*ss;
	int		c;
	char	*result;

	ss = "baubau";
	c = 'u';
	result = ft_memchr(ss, c, 7);
	printf("%p -> %c\n", result, *result);
}*/
