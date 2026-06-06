/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:34:09 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 20:01:55 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*string;

	string = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!string)
		return (NULL);
	ft_memcpy(string, s, (ft_strlen(s) + 1));
	return (string);
}

/*int main()
{
	printf("%s", ft_strdup("telefonocasa bau bau\n"));
}*/
