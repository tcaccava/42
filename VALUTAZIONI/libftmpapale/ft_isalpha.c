/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:12:33 by mpapale           #+#    #+#             */
/*   Updated: 2026/05/19 14:13:32 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

/*
	checks  for an alphabetic character; in the standard "C" locale,
	it is equivalent to (isupper(c) ||  islower(c)).   In  some  lo‐
	cales, there may be additional characters for which isalpha() is
	true—letters which are neither uppercase nor lowercase.
*/

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));

}
