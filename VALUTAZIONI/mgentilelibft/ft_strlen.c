/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 18:20:50 by mgentile          #+#    #+#             */
/*   Updated: 2026/05/20 12:29:27 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
	{
		len ++;
	}
	return (len);
}

/*int main()
{
	char str[] = "blublublui 990 90 iu";
	printf("%zu\n", ft_strlen(str));
	return (0);
}*/
