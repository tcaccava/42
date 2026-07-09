/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 05:39:19 by edveneto          #+#    #+#             */
/*   Updated: 2026/07/08 05:39:54 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*ft_reader(int fd, char *stash);
char	*ft_extractor(char *stash);
char	*ft_cleaner(char *stash);
char	*ft_strjoin(char *stash, char *buffer);
size_t	ft_strlen(const char *s);
int		ft_charcheck(char *stash, char c);

#endif