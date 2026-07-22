/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 12:07:50 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/22 16:50:04 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int	ifnegative(int n)
{
	int	i;
	int	a;

	i = 1;
	a = -1;
	while (a >= (n / 10))
	{
		a = a * 10;
		i++;
	}
	return (i);
}

int	ifpositive(int n)
{
	int	i;
	int	a;

	i = 1;
	a = 1;
	while (a <= (n / 10))
	{
		a = a * 10;
		i++;
	}
	return (i);
}

int	acalc(int i, int check)
{
	int	a;

	a = 1;
	while (i > 1)
	{
		a = a * 10;
		i--;
	}
	if (check == 2)
		a = a / 10;
	return (a);
}

char	*converter(int n, int a, int check, char *s)
{
	int	i;

	if (check == 0)
		i = 0;
	else
		i = 1;
	if (check == 2)
		n = 214748364;
	if (check == 1)
		n = -n;
	while (a >= 1)
	{
		s[i] = (n / a) + 48;
		n = n - ((n / a) * a);
		a = a / 10;
		i++;
	}
	if (check == 2)
	{
		s[i] = '8';
		i++;
	}
	s[i] = '\0';
	return (s);
}

char	*ft_itoa(int n)
{
	int		a;
	int		check;
	char	*s;

	if (n >= 0)
	{
		s = malloc(sizeof(char) * (ifpositive(n) + 1));
		if (!s)
			return (NULL);
		check = 0;
		a = acalc(ifpositive(n), check);
	}
	if (n < 0)
	{
		s = malloc(sizeof(char) * (ifnegative(n) + 2));
		if (!s)
			return (NULL);
		s[0] = '-';
		check = 1;
		if (n == -2147483648)
			check = 2;
		a = acalc(ifnegative(n), check);
	}
	converter (n, a, check, s);
	return (s);
}
/*
int	main()
{
	printf("%s\n", ft_itoa(1));
}*/
