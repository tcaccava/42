/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:35:53 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/11 15:35:58 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check(char *str)
{
	long long	number;
	int			i;

	number = 0;
	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i++])
	{
		if (str[i - 1] < '0' || str[i - 1] > '9')
			return (0);
		number = (number * 10) + (str[i - 1] - '0');
	}
	return (1);
}

void	ft_exit(char **exit_status, t_data *data, t_execute *info)
{
	if (!(*exit_status))
		data->exit_status = 0;
	else if (check(*exit_status) == 0)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(*exit_status, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		data->exit_status = 2;
	}
	else if (*(exit_status + 1) != NULL)
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		data->exit_status = 1;
	}
	else
		data->exit_status = ft_atoi(*exit_status);
	printf("exit\n");
	unlink_files(data);
	exit_and_free(data, info);
}
