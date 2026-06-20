/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:44:56 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:44:56 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		int toupper(int c);

		These functions convert lowercase letters to uppercase, and vice versa.
		If c is a lowercase letter, toupper() returns its uppercase equivalent,
		if an uppercase representation exists in the current locale.  Otherwise,
		it returns c.  The toupper_l() function performs the same task, but uses
		the locale referred to by the locale handle locale.

RETURN VALUE
		The value returned is that of the converted letter,
			or c if the conversion
		was not possible.
*/
#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		c -= 32;
	return (c);
}
