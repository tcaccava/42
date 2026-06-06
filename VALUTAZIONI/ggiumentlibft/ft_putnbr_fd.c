/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 18:26:53 by ggiument          #+#    #+#             */
/*   Updated: 2026/06/04 20:59:08 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long int	nl;

	nl = n;
	if (nl < 0)
	{
		write(fd, "-", 1);
		nl *= -1;
	}
	if (nl > 9)
		ft_putnbr_fd(nl / 10, fd);
	write(fd, &"0123456789"[nl % 10], 1);
}
