/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 16:19:13 by dd-alo            #+#    #+#             */
/*   Updated: 2026/07/28 15:01:32 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s,
				unsigned int start,
				size_t len)
{
	size_t	i;
	char	*array;
	size_t	s_len;

	i = 0;
	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (s_len <= start)
		return (ft_strdup(""));
	if (len >= s_len - start)
		len = s_len - start;
	array = malloc(sizeof(char) * len + 1);
	if (!array)
		return (NULL);
	while (i < len && s[start + i] != '\0')
	{
		array[i] = s[start + i];
		i++;
	}
	array[i] = '\0';
	return (array);
}
