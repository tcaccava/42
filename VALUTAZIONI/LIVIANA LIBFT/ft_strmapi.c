/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:27:10 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/14 18:53:43 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f) (unsigned int, char))
{
	int					len;
	char				*str;
	char				c;
	unsigned int		index;

	len = ft_strlen(s);
	index = 0;
	str = (char *) malloc ((len + 1) * (sizeof(char)));
	if (str == NULL)
		return (NULL);
	while (s[index] != '\0')
	{
		c = f(index, s[index]);
		str[index] = c;
		index++;
	}
	str[index] = '\0';
	return (str);
}

/*char	f(unsigned int i, char c)
{
	c -= 32;
	return (c);
}

int main()
{
	printf("%s", ft_strmapi("ciao", f));
}*/
