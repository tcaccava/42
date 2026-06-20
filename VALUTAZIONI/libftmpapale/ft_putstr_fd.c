/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:25 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:25 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	void ft_putstr_fd(char *s, int fd);
PARAMETERS
	s: The string to output.
	fd: The file descriptor on which to write.
DESCRIPTION
	Outputs the string ’s’ to the specified file descriptor.
RETURN VALUES
	None.
*/
#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}
