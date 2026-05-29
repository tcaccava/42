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

#include "minishell.h"

char	**ft_arrayjoin(char **s1, char **s2)
{
	int		len;
	int		i;
	char	**result;

	i = 0;
	len = array_len_norm(s1) + array_len_norm(s2) + 1;
	result = (char **) malloc (len * (sizeof(char *)));
	if (result == NULL)
		return (NULL);
	len = 0;
	while (s1 && s1[len])
	{
		result[len] = s1[len];
		len++;
	}
	while (s2 && s2[i])
	{
		result[len] = s2[i];
		len++;
		i++;
	}
	result[len] = NULL;
	return (free(s1), free(s2), result);
}
