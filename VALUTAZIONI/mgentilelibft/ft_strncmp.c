/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:35:50 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 19:47:25 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ss1;
	unsigned char	*ss2;

	ss1 = (unsigned char *)s1;
	ss2 = (unsigned char *)s2;
	i = 0;
	while ((i < n) && (s1[i] != '\0' && s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((int)ss1[i] - (int)ss2[i]);
		i++;
	}
	if (i == n)
		return (0);
	else
		return ((int)ss1[i] - (int)ss2[i]);
}

/*int	main(void)
{
	const char	*s1 = "ci";
	const char	*s2 = "ci";
	int			result;

	result = (ft_strncmp(s1, s2, 6));
	printf("%d\n", result);
}*/
