/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenzo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:26:56 by lorenzo           #+#    #+#             */
/*   Updated: 2024/08/13 18:26:58 by lorenzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *line, const char *stash)
{
	int		total;
	char	*dest;
	int		i;
	int		j;

	total = ft_strlen(line) + ft_strlen(stash) + 1;
	dest = malloc(sizeof(char) * total);
	i = 0;
	j = 0;
	while (line && line[i])
	{
		dest[j] = line[i];
		j++;
		i++;
	}
	i = 0;
	while (stash && stash[i])
	{
		dest[j] = stash[i];
		j++;
		i++;
	}
	free((char *)line);
	dest[j] = '\0';
	return (dest);
}

/*int main()
{
	printf("%s\n", ft_strjoin("ciao", "bello"));
}*/
