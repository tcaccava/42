/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 18:51:02 by ggiument          #+#    #+#             */
/*   Updated: 2026/06/05 19:13:49 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;

	start = 0;
	end = 0;
	if (!s1 || !set)
		return (NULL);
	while (ft_strchr(set, s1[start]) && s1[start])
		start++;
	while (s1[end])
		end++;
	if (end != 0)
		end--;
	while (ft_strchr(set, s1[end]) && end > start)
		end--;
	if (start > end)
		return (ft_substr(s1, start, 0));
	return (ft_substr(s1, start, end - start + 1));
}
