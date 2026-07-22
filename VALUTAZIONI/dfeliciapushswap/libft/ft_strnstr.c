/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 18:10:47 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 19:07:27 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
/*#include <stdio.h>*/

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t		i;
	size_t		n;

	i = 0;
	n = 0;
	if (!little || little[0] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && i < len)
	{
		while (big[i + n] == little[n] && (i + n < len))
		{
			n++;
			if (little[n] == '\0')
				return ((char *)&big[i]);
		}
		n = 0;
		i++;
	}
	return (NULL);
}
/*
int	main()
{
	const char *big = "oooooMARCOooooo";
	const char *little = "";
	printf("%s\n", ft_strnstr(big, little, 5));
}*/
