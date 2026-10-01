/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:07:23 by mpapale           #+#    #+#             */
/*   Updated: 2026/09/27 16:05:08 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>

char	*get_next_line(int fd);
char	*read_and_store(int fd, char *magazzino_e_extra);
char	*extract_line(char *magazzino_e_extra);
char	*obtain_remaining(char *magazzino_e_extra);
size_t	ft_strlen(const char *str);
char	*ft_strchr(const char *str, int to_find);
char	*ft_strdup(const char *src);
char	*ft_strjoin_gnl(char const *s1, char const *s2);

#endif
