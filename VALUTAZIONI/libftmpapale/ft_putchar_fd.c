/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:23 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:23 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	void ft_putchar_fd(char c, int fd);
PARAMETERS
	c: The character to output.
	fd: The file descriptor on which to write.
DESCRIPTION
	Outputs the character ’c’ to the specified
	file descriptor.
RETURN VALUES
	None.
*/
#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
