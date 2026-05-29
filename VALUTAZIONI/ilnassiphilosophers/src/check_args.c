/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 18:14:24 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/25 19:12:58 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*Returns 1 if the given string contains only digits (0-9), otherwise 0.
Used to validate CLI arguments before parsing them.*/
static int	ft_is_digits(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/*Validates parsed arguments (ranges and non-zero timing constraints).
Returns 0 on success, 1 on error and prints an explanatory message.*/
static int	check_data(t_data *data)
{
	if (data->num_philos < 1 || data->num_philos > 200)
		return (printf("Error: invalid number_of_philosophers\n"), 1);
	if (data->time_to_die <= 0
		|| data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (printf("Error: invalid time arguments\n"), 1);
	return (0);
}

/*Parses and validates command line arguments, 
then fills the t_data struct.
Handles the optional must_eat argument (set to -1 when not provided).
Returns 0 on success, 1 on error.*/
int	check_args(int argc, char **argv, t_data *data)
{
	if (argc != 5 && argc != 6)
		return (printf("Error: wrong number of arguments\n"), 1);
	if (!ft_is_digits(argv[1]) || !ft_is_digits(argv[2])
		|| !ft_is_digits(argv[3]) || !ft_is_digits(argv[4])
		|| (argc == 6 && !ft_is_digits(argv[5])))
		return (printf("Error: arguments must be positive integers\n"), 1);
	data->num_philos = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		data->must_eat = atoi(argv[5]);
	else
		data->must_eat = -1;
	return (check_data(data));
}
