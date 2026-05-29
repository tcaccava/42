/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 19:11:52 by abkhefif          #+#    #+#             */
/*   Updated: 2025/01/08 19:12:05 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putptr(void *ptr)
{
	int	len;

	if (!ptr)
		return (ft_putstr_fd("(nil)", 1));
	len = write(1, "0x", 2);
	ft_printptr((unsigned long)ptr);
	len += ft_lenptr((unsigned long)ptr);
	return (len);
}
