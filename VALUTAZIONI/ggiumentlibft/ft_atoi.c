/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:45:36 by ggiument          #+#    #+#             */
/*   Updated: 2026/05/26 16:22:43 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	segno;

	result = 0;
	segno = 1;
	while ((*str > 8 && *str < 14) || *str == 32)
		str++;
	if (*str == '-')
	{
		segno = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str)
	{
		if (*str < 48 || *str > 57)
			return (result * segno);
		result = (result * 10) + (*str - '0');
		str++;
	}
	return (result * segno);
}
