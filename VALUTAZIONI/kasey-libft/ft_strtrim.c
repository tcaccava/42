/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:39:35 by keiestre          #+#    #+#             */
/*   Updated: 2026/07/06 20:12:56 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_check(char s, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == s)
			return (1);
		i++;
	}
	return (0);
}

static	char	*ft_write(char *trimd, const char *s1, int start, int end)
{
	int	i;

	i = 0;
	while (start < end)
	{
		trimd[i] = s1[start];
		start++;
		i++;
	}
	trimd[i] = '\0';
	return (trimd);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*trimd;

	start = 0;
	end = 0;
	if (s1 == NULL || set == NULL)
		return (NULL);
	while (s1[end] != '\0')
		end++;
	while (s1[start] != '\0' && ft_check(s1[start], set))
		start++;
	while (end > start && ft_check(s1[end - 1], set))
		end--;
	trimd = (char *)malloc((end - start + 1) * sizeof(char));
	if (trimd == NULL)
		return (NULL);
	ft_write(trimd, s1, start, end);
	return (trimd);
}
