/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:05:47 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/07 01:26:27 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_echo_flag(char *str)
{
	if (str && *str == '-')
		str++;
	else
		return (1);
	while (*str)
	{
		if (*str != 'n')
			return (1);
		str++;
	}
	return (0);
}

void	ft_echo(char **str, t_data *data, t_execute *info)
{
	int	n;
	int	newline;

	set_fd(info);
	n = 0;
	data->exit_status = 0;
	newline = 1;
	if (str || str[n])
	{
		while (check_echo_flag(str[n]) == 0)
		{
			n++;
			newline = 0;
		}
		while (*str && str[n])
		{
			ft_putstr_fd(str[n], STDOUT_FILENO);
			if (str[n + 1] != NULL)
				ft_putstr_fd(" ", STDOUT_FILENO);
			n++;
		}
	}
	if (newline == 1)
		ft_putstr_fd("\n", STDOUT_FILENO);
	restore_fd(info);
}
