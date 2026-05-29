/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 04:09:32 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/16 22:38:49 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	new_len;

	if ((int)start < 0 || !s
		|| start >= ft_strlen(s))
		return (ft_strdup(""));
	new_len = 0;
	while (s[start + new_len] != '\0' && new_len < len)
		new_len++;
	new = (char *) malloc(sizeof(char) * (new_len + 1));
	if (new == NULL)
		return (NULL);
	ft_strlcpy(new, &(s[start]), new_len + 1);
	return (new);
}
