/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printfnbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcalleja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 18:34:09 by mcalleja          #+#    #+#             */
/*   Updated: 2026/06/12 19:05:52 by mcalleja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printfnbr(int n)
{
	int		len;
	char	*str;

	str = ft_itoa(n);
	len = ft_putstr_len(str);
	free(str);
	return (len);
}
