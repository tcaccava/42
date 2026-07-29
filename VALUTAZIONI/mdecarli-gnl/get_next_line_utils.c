/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdecarli <mdecarli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 17:07:45 by mdecarli          #+#    #+#             */
/*   Updated: 2026/07/01 16:32:51 by mdecarli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	char	*new_str;
	int		i;

	i = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!new_str)
		return (NULL);
	while (s[i] != '\0')
	{
		new_str[i] = s[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_strjoin_gnl(char *storage, char *buffer)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	if (!storage)
		storage = ft_strdup("");
	if (!storage || !buffer)
		return (NULL);
	new_str = malloc(sizeof(char)
			* (ft_strlen(storage) + ft_strlen(buffer) + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (storage[i])
		new_str[j++] = storage[i++];
	i = 0;
	while (buffer[i])
		new_str[j++] = buffer[i++];
	new_str[j] = '\0';
	free(storage);
	return (new_str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_str;
	size_t	i;

	i = 0;
	new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	while (i < len)
	{
		new_str[i] = s[start];
		i++;
		start++;
	}
	new_str[i] = '\0';
	return (new_str);
}
