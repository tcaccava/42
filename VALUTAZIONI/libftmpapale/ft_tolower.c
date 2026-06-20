/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:45:29 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:45:29 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
int	tolower(int c);
These functions convert lowercase letters to uppercase, and vice versa.

		If c is an uppercase letter, tolower() returns its lowercase equivalent,
		if a lowercase representation exists in the current locale.  Otherwise,
		it returns c.  The tolower_l() function performs the same task, but uses
		the locale referred to by the locale handle locale.

		RETURN VALUE
		The value returned is that of the converted letter,
			or c if the conversion
		was not possible.
*/
#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 65 && c <= 90)
		c += 32;
	return (c);
}
