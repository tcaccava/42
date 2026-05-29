/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_unint.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:40:32 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/08 16:43:02 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_count_unint(unsigned int n, int *count)
{
	unsigned long long	i;

	i = n;
	ft_count_putnbr_base(i, DECIMAL, count);
}
