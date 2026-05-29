/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzecchin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:29:54 by lzecchin          #+#    #+#             */
/*   Updated: 2025/04/28 18:29:57 by lzecchin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 29
# endif

# define MAX_FD 1024

char	*get_next_line(int fd);
size_t	ft_strlen_0(const char *s);
char	*ft_strchr(const char *s, int c);
void	*ft_memcpy_0term(void *dst, const void *src, size_t n);
char	*ft_concatenate(char **saved, char *buffer, int n);
char	*ft_getline_update_saved(char **saved, int n);

#endif
