/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:02:43 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/14 18:38:36 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_scheduler(char *str, t_scheduler *scheduler)
{
	if (strcmp(str, "fifo") == 0)
		*scheduler = FIFO;
	else if (strcmp(str, "edf") == 0)
		*scheduler = EDF;
	else
		return (0);
	return (1);
}

static int	check_parse_input(t_global_data *data)
{
	if (data->number_of_coders <= 0
		|| data->time_to_burnout <= 0
		|| data->time_to_compile <= 0
		|| data->time_to_debug <= 0
		|| data->time_to_refactor <= 0
		|| data->number_of_compiles_required <= 0
		|| data->dongle_cooldown < 0)
		return (0);
	return (1);
}

static int	parse_args(char **argv, t_global_data *data)
{
	data->stop_simulation = 0;
	if (!ft_safe_atoi(argv[1], &data->number_of_coders)
		|| !ft_safe_atol(argv[2], &data->time_to_burnout)
		|| !ft_safe_atol(argv[3], &data->time_to_compile)
		|| !ft_safe_atol(argv[4], &data->time_to_debug)
		|| !ft_safe_atol(argv[5], &data->time_to_refactor)
		|| !ft_safe_atoi(argv[6], &data->number_of_compiles_required)
		|| !ft_safe_atol(argv[7], &data->dongle_cooldown)
		|| !parse_scheduler(argv[8], &data->scheduler))
		return (0);
	return (check_parse_input(data));
}

int	check_args(int argc, char **argv, t_global_data *data)
{
	if (argc != 9)
	{
		fprintf(stderr, "ERROR: Argument count does not match\n");
		return (0);
	}
	if (!parse_args(argv, data))
	{
		fprintf(stderr, "ERROR: Parsing the arguments failed\n");
		return (0);
	}
	return (1);
}
