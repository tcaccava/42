/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcorasan <kcorasan@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 03:37:12 by sara              #+#    #+#             */
/*   Updated: 2026/09/23 18:56:50 by kcorasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	count_flags(int argc, char **argv)
{
	int	i;
	int	count;

	i = 1;
	count = 0;
	while (i < argc && ft_strncmp(argv[i], "--", 2) == 0)
	{
		count++;
		i++;
	}
	return (count);
}

static int	validate(char **argv, int total)
{
	if (has_flag(argv, "--bench"))
		total--;
	if (has_flag(argv, "--simple"))
		total--;
	else if (has_flag(argv, "--medium"))
		total--;
	else if (has_flag(argv, "--complex"))
		total--;
	else if (has_flag(argv, "--adaptive"))
		total--;
	if (total == 0)
		return (1);
	else
		return (0);
}

char	**build_numbers(int argc, char **argv)
{
	int	flags;

	flags = count_flags(argc, argv);
	if (validate(argv, flags) == 0)
		return (NULL);
	if (flags > 0)
		return (create_numbers_array(argc - flags, argv + flags));
	return (create_numbers_array(argc, argv));
}

int	has_flag(char **argv, char *flag)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_strncmp(argv[i], flag, ft_strlen(flag)) == 0)
			return (1);
		i++;
	}
	return (0);
}
