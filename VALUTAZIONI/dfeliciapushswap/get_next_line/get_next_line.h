/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:22:07 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/15 15:56:25 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

int		readlen(char *s);
char	*readjoin(char *s1, char *s2);
int		nlfinder(char *s);
char	*stashpointer(char *stash);
char	*liner(char *stash);
char	*stashmaker(char *stash, char *buffer, int fd);
char	*freethebuffer(char *buffer, char *returned);
char	*get_next_line(int fd);

#endif
