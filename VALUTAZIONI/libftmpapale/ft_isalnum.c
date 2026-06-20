/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:15:33 by mpapale           #+#    #+#             */
/*   Updated: 2026/05/19 14:15:36 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

/*checks for an alphanumeric character; it is equivalent to
(isal‐pha(c) ||isdigit(c)).*/

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	else
		return (0);
}
