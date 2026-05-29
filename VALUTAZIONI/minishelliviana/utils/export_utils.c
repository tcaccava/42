/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:05:24 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/17 19:05:25 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_export_variable(char *str)
{
	int	n;

	n = 0;
	while (str[n] != '=' && str[n] != '\0')
	{
		if (str[n] == '+' && str[n + 1] == '=')
			break ;
		n++;
	}
	return (ft_substr(str, 0, n));
}

char	*export_join(char *str)
{
	int		i;
	char	*variable;
	char	*value;
	char	*temp;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (ft_strdup(str));
		if (str[i] == '+')
			break ;
		i++;
	}
	if (str[i] == '+')
	{
		variable = get_export_variable(str);
		value = get_value(str);
		temp = ft_strjoin(variable, "=");
		str = ft_strjoin(temp, value);
		free(temp);
		free(variable);
		free(value);
	}
	return (ft_strdup(str));
}

char	*ft_strjoin2(char *s1, char *s2)
{
	int		len;
	char	*string;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	string = (char *) malloc (len * (sizeof(char)));
	if (string == NULL)
		return (NULL);
	ft_strlcpy(string, s1, len);
	ft_strlcat(string, s2, len);
	free(s1);
	free(s2);
	return (string);
}
