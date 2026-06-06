/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 20:26:26 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/02 19:40:19 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if ((unsigned char)s[i] == (unsigned char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)&s[i]);
	else
		return (NULL);
}
/*int main()
{
	const char *s = "ciao ciao";
	int	c = '\0';
	char *result = ft_strchr(s, c);
	if (result != NULL)
		printf("trovata occorrenza: %s\n", result);
	else
		printf("non trovata occorrenza\n");
}*/
