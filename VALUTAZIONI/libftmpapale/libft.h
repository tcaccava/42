/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:43:31 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:43:31 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;
// check character
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);

// string handling
size_t				ft_strlen(const char *str);
// lenght of a string
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
// copy safely
size_t				ft_strlcat(char *dest, const char *src, size_t size);
// concatenate safely
char				*ft_strchr(const char *str, int to_find);
// search the first occurrence
char				*ft_strrchr(const char *str, int to_find);
// search the last occurrence
int					ft_strncmp(const char *s1, const char *s2, size_t n);
// compares two strings in a N limit,
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
// search for a string in a limit*/ ho solo strstr

// memory
void				*ft_memset(void *s, int c, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memchr(const void *src, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);

// case conversion
int					ft_toupper(int c);
int					ft_tolower(int c);

// to
int					ft_atoi(const char *str);
char				*ft_itoa(int n);

// malloc
void				*ft_calloc(size_t n, size_t size);
char				*ft_strdup(const char *src);

// use function
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));

// write
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

// more string
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);

// linked list
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

#endif
