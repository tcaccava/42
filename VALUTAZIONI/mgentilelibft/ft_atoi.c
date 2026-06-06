/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:03:15 by mgentile          #+#    #+#             */
/*   Updated: 2026/05/27 13:09:49 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_atoi(const char *nptr)
{
	int	nb;
	int	i;
	int	check;

	i = 0;
	nb = 0;
	check = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || (nptr[i] == ' '))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			check++;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (check == 1)
			nb = (nb * 10) - (nptr[i] - 48);
		else
			nb = (nb * 10) + (nptr[i] - 48);
		i++;
	}
	return (nb);
}

/*int	main(void)
{
	printf("%d", ft_atoi("    ++uuywueirui"));
}*/
