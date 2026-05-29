/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadferna <marvin.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:11:02 by kadferna          #+#    #+#             */
/*   Updated: 2025/05/14 14:11:03 by kadferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	check_each_arg(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!has_letters(argv[i]))
			return (std_error("must not contain any letters"), 0);
		i++;
	}
	return (1);
}

int	checks(int *nums, int argc, char **argv)
{
	if (!(argc == 5 || argc == 6))
		return (cust_error(), 0);
	if (!check_each_arg(argc, argv))
		return (0);
	if (!check_numbers(nums, argc, argv))
		return (std_error("All the inputs must be greater than 0"), 0);
	if (!check_num_values(nums))
		return (0);
	return (1);
}

int	check_num_values(int *nums)
{
	if (nums[0] > MAX_PHILOS)
		return (printf("Error\nMaximum nums of philosophers is %d\n",
				MAX_PHILOS), 0);
	else if (nums[0] < 1)
		return (std_error("There should be atleast 1 philosopher"), 0);
	else if (nums[1] < 0 || nums[2] < 0 || nums[3] < 0)
		return (std_error(NOT_GREATER_0), 0);
	return (1);
}

t_data	*do_checks_and_parse(t_philo *philos, int *nums)
{
	t_data	*data;

	if (!check_num_values(nums))
		return (NULL);
	data = init(nums, &philos);
	if (!data)
		return (NULL);
	return (data);
}
