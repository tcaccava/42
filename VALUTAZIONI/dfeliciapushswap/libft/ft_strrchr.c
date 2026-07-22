/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 17:28:27 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 18:20:53 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>*/
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int			i;
	const char	*pos;

	i = 0;
	pos = NULL;
	if (s)
	{
		while (s[i] != '\0')
		{
			if (s[i] == (unsigned char)c)
				pos = &s[i];
			i++;
		}
		if (s[i] == (unsigned char)c)
			pos = &s[i];
		return ((char *)pos);
	}
	return ((char *) pos);
}
/*
int	main()
{
	char	*s = "morco";
	printf("%s\n",ft_strrchr(s, 'o'));
}*/
