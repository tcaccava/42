/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:41:57 by mgentile          #+#    #+#             */
/*   Updated: 2026/05/26 16:02:52 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!little[0])
		return ((char *)big);
	while ((big[i]) && (i < len))
	{
		j = 0;
		while (little[j] && (big[i + j] == little[j]) && (i + j) < len)
			j++;
		if (little[j] == '\0')
			return ((char *)big + i);
		i++;
	}
	return (NULL);
}

/*int main()
{
	const char *pd = "ciaobaubaubro ciao";
	const char *little = "bro";

	printf("%s", ft_strnstr(pd, little, 4));
}*/
