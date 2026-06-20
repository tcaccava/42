/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:16:34 by mpapale           #+#    #+#             */
/*   Updated: 2026/05/19 14:16:43 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

// checks for any printable character including space.
int	ft_isprint(int c)
{
	if ((c >= 32 && c <= 126))
		return (1);
	else
		return (0);
}
