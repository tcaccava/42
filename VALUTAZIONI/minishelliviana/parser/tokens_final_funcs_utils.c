/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_final_funcs_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:26:32 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 17:26:34 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	forbidden_symbols(char c)
{
	const char	*special_symbols = "-!@$#%^&*()+={}[]|\\:;\"'<>,.? ";

	if (ft_strchr(special_symbols, c) != NULL)
		return (1);
	return (0);
}

char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*replace_range(char *s1, char *s2, size_t i, size_t j)
{
	size_t	len1;
	size_t	len2;
	size_t	new_len;
	char	*result;

	len1 = ft_strlen(s1);
	if (i > j || j >= len1)
		return (NULL);
	len2 = ft_strlen(s2);
	new_len = i + len2 + (len1 - j - 1);
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, s1, i);
	ft_strcpy(result + i, s2);
	ft_strcpy(result + i + len2, s1 + j + 1);
	free(s1);
	free(s2);
	return (result);
}
