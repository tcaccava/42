/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:57:43 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 19:49:02 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substring;
	size_t	remaining;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
	{
		substring = malloc(1);
		if (!substring)
			return (NULL);
		substring[0] = '\0';
		return (substring);
	}
	remaining = ft_strlen(s) - start;
	if (len > remaining)
		len = remaining;
	substring = malloc((len + 1) * sizeof(char));
	if (!substring)
		return (NULL);
	ft_strlcpy(substring, s + start, len + 1);
	return (substring);
}
