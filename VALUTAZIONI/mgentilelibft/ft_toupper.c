/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 19:36:29 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 16:27:09 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	int	result;

	result = c;
	if (c >= 'a' && c <= 'z')
		result = c - 32;
	return (result);
}

/*int main()
{
	int c = 112;
	c = ft_toupper(c);
	printf("%d\n", c);
}*/
