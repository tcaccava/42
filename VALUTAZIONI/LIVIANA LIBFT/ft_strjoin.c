/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:10:19 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/11/15 13:12:31 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	char	*string;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	string = (char *) malloc (len * (sizeof(char)));
	if (string == NULL)
		return (NULL);
	ft_strlcpy(string, s1, len);
	ft_strlcat(string, s2, len);
	return (string);
}

/*int main(int ac, char **av)
{
	printf("%s", ft_strjoin(av[1], av[2]));
}*/
