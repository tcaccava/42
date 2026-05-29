/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:59:27 by abkhefif          #+#    #+#             */
/*   Updated: 2025/01/08 19:00:14 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr(int i)
{
	char	*nb;
	int		len;

	nb = ft_itoaa(i);
	if (!nb)
		return (0);
	len = ft_putstr(nb);
	free(nb);
	return (len);
}
