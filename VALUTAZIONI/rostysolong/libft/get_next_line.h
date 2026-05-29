/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:12:49 by rodolhop          #+#    #+#             */
/*   Updated: 2025/03/03 14:18:31 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1234
# endif

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
#include "libft.h"

char	*get_next_line(int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_read_buffer(int fd, char *text);
char	*ft_extract_line(char *text);
char	*ft_trim_buffer(char *text);

#endif