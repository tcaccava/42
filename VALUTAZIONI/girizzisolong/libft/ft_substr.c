/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:43:53 by girizzi           #+#    #+#             */
/*   Updated: 2025/01/04 19:32:39 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char			*new;
	unsigned int	i;
	size_t			len_str;

	i = -1;
	if (!s)
		return (NULL);
	len_str = ft_strlen(s);
	if (len > len_str)
		len = len_str;
	if (len == 0 || start >= len_str)
		len = 0;
	else if (start + len > len_str)
		len = len_str - start;
	new = (char *) malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	while (++i < len && s[i] != '\0')
		new[i] = s[start + i];
	new[i] = '\0';
	return (new);
}
