/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_push_swap.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 12:06:48 by dmoda             #+#    #+#             */
/*   Updated: 2026/08/11 14:29:00 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PUSH_SWAP_H
# define FT_PUSH_SWAP_H

# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct t_dlist
{
	int				cont;
	struct t_dlist	*next;
	struct t_dlist	*prev;
}	t_dlist;

int		ft_is_number(char *num);
int		ft_create_list(t_dlist **head_a, int argc, char **argv);
t_dlist	**ft_new_head(void);
t_dlist	*ft_new_node(int cont);
int		ft_add_node(t_dlist **head_dst, t_dlist *node);
void	ft_free_list(t_dlist **head);
int		ft_stack_len(t_dlist *head);
int		ft_sqrt(int n);
int		ft_max(int a, int b);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_no_duplicate(int argc, char **argv);
int		ft_atoi(const char *str);
float	ft_disorder(t_dlist *a, int len);
void	ft_rotate(t_dlist **head_a, t_dlist **head_b, int rotation, char c);
void	ft_push(t_dlist **head_dst, t_dlist **head_src, char c);
void	ft_swap(t_dlist **head, t_dlist **head_snd, char c);
void	ft_bubble(t_dlist **head_a, t_dlist **head_b);
void	ft_quick(t_dlist **head_a, t_dlist **head_b, int len);
void	ft_rev_quick(t_dlist **head_a, t_dlist **head_b, int len);
void	ft_block(t_dlist **head_a, t_dlist **head_b, int chunk_size);
int		ft_bench_sp(int mode, char c);
int		ft_bench_r(int mode, char c);
int		ft_check_flag(char *str);
int		ft_putnbr_fd(int n, int fd);
int		ft_printf(int fd, const char *format, ...);
int		ft_print_bench(float d, char *str1, char *str2, int len);
int		ft_condition(t_dlist **a, t_dlist **b);
int		ft_is_overflow(char *num);
size_t	ft_strlen(const char *s);

#endif
