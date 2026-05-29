/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:39:40 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/14 18:39:35 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f) (unsigned int, char*))
{
	unsigned int		index;

	index = 0;
	while (s[index] != '\0')
	{
		f(index, s + index);
		index++;
	}
}

/*void	f(unsigned int i, char *c)
{
	c[i] = c[i] + 32;
}

int main()
{
	char	str[] = "CIAO";

	ft_striteri(str, f);
	printf("%s", str);
}*/
