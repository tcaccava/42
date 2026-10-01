/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 16:12:27 by sara              #+#    #+#             */
/*   Updated: 2026/07/27 12:26:31 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

// conta quanti numeri ci sono in totale tra gli argv
int	count_numbers(int argc, char **argv)
{
	int	i;
	int	count;
	int	res;

	i = 1;
	count = 0;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "--", 2) == 0)
			i++;
		else
		{
			res = count_split(argv, i);
			if (!res)
				return (0);
			count += res;
			i++;
		}
	}
	return (count);
}

// diverso free, libera solo memoria utilizzata,
// per non andare a toccare quella non INIZIALIZZATA
static void	free_numbers(char **numbers, int used)
{
	int	i;

	i = 0;
	while (i < used)
	{
		free(numbers[i]);
		i++;
	}
	free(numbers);
}

// copia i token di uno spolit dentro l'array finale, quindi numbers,
// avanzando pos_n
static int	copy_split(char **numbers, char **split, int *pos_n)
{
	int	j_split;

	j_split = 0;
	while (split[j_split])
	{
		numbers[*pos_n] = ft_strdup(split[j_split]);
		if (!numbers[*pos_n])
		{
			numbers[*pos_n] = NULL;
			free_split(split);
			free_numbers(numbers, *pos_n);
			return (0);
		}
		(*pos_n)++;
		j_split++;
	}
	return (1);
}

// creo un contenitore che conterrà i numeri passati
// in modo pulito
char	**create_numbers_array(int argc, char **argv)
{
	char	**numbers;
	char	**split;
	int		total;
	int		i_av;
	int		pos_n;

	total = count_numbers(argc, argv);
	if (!total)
		return (NULL);
	numbers = malloc(sizeof(char *) * (total + 1));
	if (!numbers)
		return (NULL);
	i_av = 1;
	pos_n = 0;
	while (i_av < argc)
	{
		split = ft_split(argv[i_av++], ' ');
		if (!split)
			return (free_numbers(numbers, pos_n), NULL);
		if (!copy_split(numbers, split, &pos_n))
			return (NULL);
		free_split(split);
	}
	numbers[pos_n] = NULL;
	return (numbers);
}
