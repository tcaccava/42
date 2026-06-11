/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdecarli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:22:28 by mdecarli          #+#    #+#             */
/*   Updated: 2026/05/26 17:50:52 by mdecarli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *a, const char *b, size_t n)
{
	size_t		i;
	size_t		j;

	i = 0;
	if (b[0] == '\0')
		return ((char *)a);
	while (a[i] && i < n)
	{
		j = 0;
		while (a[i + j] && b[j] && (i + j < n) && a[i + j] == b[j])
		{
			j++;
		}
		if (b[j] == '\0')
		{
			return ((char *)(a + i));
		}
		i++;
	}
	return (NULL);
}

/*int	main(void)
{
	char	*a = "ciao Miao 42";
	char	*res;

	res = ft_strnstr(a, "42", 20);
	printf("%s\n", res);
	return (0);
}*/
