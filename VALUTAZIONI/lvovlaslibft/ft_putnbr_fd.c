/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 20:57:56 by lvovlas           #+#    #+#             */
/*   Updated: 2026/06/08 12:27:15 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	*num_char;

	num_char = ft_itoa(n);
	if (!num_char)
		return ;
	write(fd, num_char, ft_strlen(num_char));
	free(num_char);
}
