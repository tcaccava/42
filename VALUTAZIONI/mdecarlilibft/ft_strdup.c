/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdecarli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:06:01 by mdecarli          #+#    #+#             */
/*   Updated: 2026/05/27 18:04:37 by mdecarli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s);
void	*ft_memcpy(void *dst, const void *src, size_t n);

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	len;

	len = ft_strlen(s1);
	copy = (char *)malloc(sizeof(char) * (len + 1));
	if (copy == NULL)
		return (NULL);
	ft_memcpy(copy, s1, len);
	copy [len] = '\0';
	return (copy);
}

/*int	main(void)
{
	char *original = "ciao miao 42";
	char *duplicate = ft_strdup(original);

	printf("%s\n", original);
	printf("%s\n", duplicate);
	free(duplicate);
	return (0);
}*/
