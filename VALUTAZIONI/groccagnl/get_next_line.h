/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grocca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:56:44 by grocca            #+#    #+#             */
/*   Updated: 2026/07/01 16:56:48 by grocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	int				fd;
	char			*buf;
	struct s_list	*next;
}	t_list;

char	*substruntil(char *s, int len);
void	strshift(char *s, int j);
size_t	findchr(char *s, char c);
char	*join(char *s1, char *s2);
t_list	*new_node(int fd);
t_list	*list(t_list *lst, int fd);
char	*free_list(t_list *lst, int fd);
char	*get_next_line(int fd);

#endif
