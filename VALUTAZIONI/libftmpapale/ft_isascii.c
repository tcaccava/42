/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:15:47 by mpapale           #+#    #+#             */
/*   Updated: 2026/05/19 14:15:49 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

/*checks whether c is a 7-bit unsigned char value that
  fits  into the ASCII character set.*/
int	ft_isascii(int c)
{
	if ((c >= 0 && c <= 127))
		return (1);
	else
		return (0);
}
