/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 18:31:52 by lvovlas           #+#    #+#             */
/*   Updated: 2026/07/18 13:20:16 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	n_ln;

	n_ln = ft_strlen((needle));
	if (!len && *haystack && *needle)
		return (NULL);
	if (len <= 0 || !haystack)
		return ((char *) haystack);
	if (!n_ln)
		return ((char *) haystack);
	while (*haystack && ft_strncmp(needle, haystack, n_ln) && len && n_ln < len)
	{
		haystack++;
		len--;
	}
	if (len <= 0 || ft_strncmp(needle, haystack, n_ln))
		return (NULL);
	return ((char *) haystack);
}
