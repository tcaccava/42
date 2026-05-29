/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 19:20:33 by abkhefif          #+#    #+#             */
/*   Updated: 2025/01/08 19:20:43 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunbr_fd(unsigned int n, int fd)
{
	int		i;

	i = n;
	if (n / 10)
		ft_putunbr(n / 10);
	ft_putchar(n % 10 + '0');
	return (ft_lenunbr(i));
}
