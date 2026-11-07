/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:32:48 by dd-alo            #+#    #+#             */
/*   Updated: 2026/07/28 19:05:37 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *lil, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (lil[0] == 0)
		return ((char *)big);
	while (i < len && big[i])
	{
		j = 0;
		while ((big[i + j] == lil[j]) && lil[j] && (i + j) < len)
		{
			j++;
		}
		if (lil[j] == 0)
		{
			return ((char *)&big[i]);
		}
		i++;
	}
	return (NULL);
}

// If little is an empty string, big is returned

//  if little occurs nowhere  in big, NULL is returned;

// other wise a pointer to the first charactor
// of the first occurence of litte is returned