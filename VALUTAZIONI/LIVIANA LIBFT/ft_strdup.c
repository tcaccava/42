/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 19:03:59 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 17:33:42 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*duplicate;
	int		len;

	len = ft_strlen(s);
	duplicate = (char *) malloc ((len + 1) * (sizeof (char)));
	ft_strlcpy(duplicate, s, len + 1);
	return (duplicate);
}

/*int main()
{
	printf("%s", ft_strdup("-2146483648"));
}*/
