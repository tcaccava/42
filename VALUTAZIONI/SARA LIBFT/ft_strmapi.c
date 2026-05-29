/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:59:57 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 15:59:58 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	char			*f_str;
	int				len;
	unsigned int	i;

	i = 0;
	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	f_str = (char *) malloc(sizeof(char) * (len + 1));
	if (!f_str)
		return (NULL);
	while (s[i] != '\0')
	{
		f_str[i] = (*f)(i, s[i]);
		i ++;
	}
	f_str[i] = '\0';
	return (f_str);
}
