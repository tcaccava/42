/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:42:54 by girizzi           #+#    #+#             */
/*   Updated: 2025/03/05 16:58:33 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

char	*get_next_line(int fd);
char	*fill_buffer(int fd, char *buffer);
char	*fill_line(char *str);
char	*clean_line(char *line);
char	*ft_strchr(const char *str, int c);
char	*ft_strjoin(char *str1, char *str2);
size_t	ft_strlen(const char *str);

#endif