/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:07:34 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 13:11:18 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*last;

	i = 0;
	last = NULL;
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			last = (char *)(s + i);
		i++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)s + i);
	return (last);
}

/*int	main(void)
{
	const char	*s = "mamma mia";
	int	c;
	char	*last;

	c = '\0';
	last = ft_strrchr(s, c);
	if (last != NULL)
		printf("last match: %s\n", last);
	else
		printf("no match");
}*/
