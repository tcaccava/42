/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:34:46 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/19 21:31:21 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*set_prompt(void)
{
	char	*buf;
	char	*start;
	int		len;
	char	*prompt;

	buf = malloc(1024 * sizeof(char));
	getcwd(buf, 1024);
	len = ft_strlen(buf);
	start = buf + len;
	while (*start != '/')
	{
		start--;
		len--;
	}
	prompt = ft_substr(buf, len, ft_strlen(start));
	free(buf);
	buf = NULL;
	buf = ft_strjoin(prompt, " ---> ");
	return (free(prompt), buf);
}
