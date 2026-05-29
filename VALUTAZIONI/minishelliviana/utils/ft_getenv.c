/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:24:19 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/19 15:42:43 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(char *str)
{
	int	n;

	n = 0;
	while (str[n] != '=')
	{
		if (str[n] == '\0')
			return (ft_strdup("\0"));
		n++;
	}
	return (ft_substr(str, n + 1, ft_strlen(str) - n));
}

char	*ft_getenv(char *variable, char **env)
{
	char	*value;
	int		len;
	int		i;

	value = NULL;
	len = ft_strlen(variable);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(variable, env[i], len) == 0 && \
		(env[i][len] == '=' || env[i][len] == '\0'))
		{
			value = get_value(env[i]);
		}
		i++;
	}
	if (value && ft_strlen(value) == 0)
		return (free(value), ft_strdup(""));
	return (value);
}
