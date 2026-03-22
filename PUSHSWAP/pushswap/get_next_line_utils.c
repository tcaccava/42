/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 17:57:34 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 15:17:03 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
/*
int	main(void)
{
	char	test[] = ("Hello world");
	size_t	length;

	length = ft_strlen(test);
	printf("length = %zu\n", length);
	return (0);
}
*/

char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	dup = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!dup)
		return (NULL);
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
/*
int	main(void)
{
	char	test[] = ("Hello World");
	char	*dup;

	dup = ft_strdup(test);
	printf("duplicate = %s\n", dup);
}
*/

char	*ft_strjoin(char *s1, char *s2)
{
	char	*join;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	join = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy(join + ft_strlen(s1), s2);
	return (join);
}
/*
int	main(void)
{
	char test1[] = ("Hello ");
	char test2[] = ("World");
	char *join = ft_strjoin(test1, test2);
	printf("join = %s\n", join);
	return (0);
}
*/
/*
char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	if (!s)
		return (NULL);
	substr = (char *)malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	while (s[i])
	{
		if (i >= start && i < start + len)
		{
			substr[j] = s[i];
			j++;
		}
		i++;
	}
	substr[j] = '\0';
	return (substr);
}
*/
/*
int main()
{
	char test[] = "Hello @World ciaone";
	printf("substring = %s\n", ft_substr(test, 0, 8));
	printf("substring = %s\n", ft_substr(test, 26, 10));
	printf("substring = %s\n", ft_substr(test, 12, 100));
	return (0);
}
*/

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (0);
}
/*
int main()
{
	char test[] = "Ciaone";
	printf("first occurrence: %s", ft_strchr(test,'a'));
	return (0);
}
*/

char	*ft_strcpy(char *dest, const char *src)
{
	char	*original_dest;

	original_dest = dest;
	if (!src)
		return (NULL);
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (original_dest);
}
/*
int main()
{
	char src[] = "Ciaone";
	char dest[50];
	ft_strcpy(dest, src);
	printf("dest: %s", dest);
}
*/
