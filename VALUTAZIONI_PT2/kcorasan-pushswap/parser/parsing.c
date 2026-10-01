/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:53:29 by sara              #+#    #+#             */
/*   Updated: 2026/07/28 05:10:49 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>

// gestisce i casi limite
int	check_range(char *str)
{
	if (str[0] == '+')
	{
		if (ft_strlen(str) == 11)
			if (ft_strncmp(str + 1, "2147483647", 10) > 0)
				return (0);
	}
	if (str[0] == '-')
	{
		if (ft_strlen(str) == 11)
			if (ft_strncmp(str + 1, "2147483648", 10) > 0)
				return (0);
	}
	else if (str[0] != '+' && str[0] != '-')
	{
		if (ft_strlen(str) == 10)
			if (ft_strncmp(str, "2147483647", 10) > 0)
				return (0);
	}
	return (1);
}

// se prendo int massimo che ha 10 numeri, con segno 11
//è troppo grande per stare in un int e andrebbe in overflow
// quando converto con ft_atoi
int	limit_case(char *str)
{
	if (str[0] == '+' || str[0] == '-')
	{
		if (ft_strlen(str) > 11)
			return (0);
	}
	else
	{
		if (ft_strlen(str) > 10)
			return (0);
	}
	return (check_range(str));
}

// controlla che la stringa sia un intero valido,
// // fatta di cifre con segno se c'è
int	valid_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
	{
		i++;
		if (str[i] < '0' || str[i] > '9')
			return (0);
	}
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	if (str[i] != '\0')
		return (0);
	if (limit_case(str) == 0)
		return (0);
	return (1);
}

// scorre tutti gli argomenti e controlla tramite validnum se sono validi,
// altrimenti stampa errore

int	pars_args(char **numbers)
{
	int	i;

	i = 0;
	while (numbers[i])
	{
		if (!valid_num(numbers[i]))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

// controlla che non ci siano dubplicati
int	not_double(char **numbers)
{
	int	i;
	int	j;

	i = 0;
	while (numbers[i])
	{
		j = i + 1;
		while (numbers[j])
		{
			if (ft_atoi(numbers[i]) == ft_atoi(numbers[j]))
			{
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
