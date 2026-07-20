/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 15:05:42 by lvovlas           #+#    #+#             */
/*   Updated: 2026/06/08 12:22:10 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	size_t	s1_len;

	s1_len = ft_strlen(s1) + 1;
	ptr = (char *) malloc(s1_len * sizeof(char));
	if (!ptr)
	{
		return (NULL);
	}
	ft_memcpy(ptr, s1, s1_len);
	return (ptr);
}
