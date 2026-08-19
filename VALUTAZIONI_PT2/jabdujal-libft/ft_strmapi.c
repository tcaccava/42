/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabdujal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:17:55 by jabdujal          #+#    #+#             */
/*   Updated: 2026/08/17 18:31:02 by jabdujal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	len;
	size_t	j;
	char	*res;

	len = ft_strlen(s);
	res = malloc((len + 1) * sizeof(*res));
	if (!res)
		return (NULL);
	j = 0;
	while (j < len)
	{
		res[j] = f(j, s[j]);
		j++;
	}
	res[j] = '\0';
	return (res);
}
