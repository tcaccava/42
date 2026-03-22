/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 13:56:22 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/24 13:37:25 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42

# endif

typedef struct s_stack
{
	int				value;
	int				index;
	int				pos;
	int				target_pos;
	int				cost_a;
	int				cost_b;
	int				move_count;
	int				cheapest_move;
	struct s_stack	*next;
	struct s_stack	*prev;
}					t_stack;

void				sort_3(t_stack **stack);
int					find_min(t_stack *stack);
void				move_to_top(t_stack **stack, int value);
int					get_stack_size(t_stack *stack);
void				sort_5(t_stack **a, t_stack **b);
int					ft_atoi(const char *str);
int					is_int(char *str);
int					is_valid_int(char *str);
void				write_error(void);
void				check_duplicates(t_stack *stack);
t_stack				*validate_input(int argc, char **argv);
void				pa(t_stack **a, t_stack **b);
void				pb(t_stack **a, t_stack **b);
void				rra(t_stack **a);
void				rrb(t_stack **b);
void				rrr(t_stack **a, t_stack **b);
void				ra(t_stack **a);
void				rb(t_stack **b);
void				rr(t_stack **a, t_stack **b);
t_stack				*create_node(int value);
void				add_to_stack(t_stack **stack, int value);
void				sa(t_stack *a);
void				sb(t_stack *b);
void				ss(t_stack *a, t_stack *b);
void				ft_formatspecifier(char specifier, va_list args,
						int *length);
int					ft_printf(const char *print, ...);
void				ft_putnbr(int n, int *length);
void				ft_putchar(char c, int *length);
void				ft_putstr(char *s, int *length);
char				**ft_split(const char *str, char delimiter);
char				**split_words(const char *str, char delimiter, char **split,
						int *i);
int					find_next_min(t_stack *stack, int min_val);
void				set_index(t_stack **stack, int size);
void				set_position(t_stack **stack);
int					find_target_pos1(t_stack *a, int b_value);
int					find_target_pos2(t_stack *a);
void				set_target_positions(t_stack **a, t_stack **b);
void				move_to_target_position(t_stack **a, t_stack **b);

void				set_above_median(t_stack *stack, int median);
void				set_cheapest_move(t_stack *a, t_stack *b,
						t_stack **cheapest);
int					calculate_moves(t_stack *a, t_stack *b, t_stack *b_node);
t_stack				*find_cheapest_move(t_stack *b);

void				push_to_b(t_stack **a, t_stack **b);
int					find_median(t_stack *stack);

void				recalc_targets(t_stack **a, t_stack **b);
void				increment_move_count(void);
int					get_move_count(void);
void				print_stack(t_stack *stack);
int					find_min_position(t_stack *stack);
int					find_max_position(t_stack *stack);
int					find_target_position(t_stack *a, int b_value);
void				calculate_cost(t_stack **a, t_stack **b);
void				execute_rotation(t_stack **a, t_stack **b, int cost_a,
						int cost_b);
void				rotate_to_min(t_stack **a);
void				execute_cheapest_move(t_stack **a, t_stack **b);
void				sort_large(t_stack **a, t_stack **b);
int					abs(int n);
void				initial_push_to_b(t_stack **a, t_stack **b);
int					is_sorted(t_stack *stack);
void				push_chunks(t_stack **a, t_stack **b, int size);
int					find_target_position(t_stack *a, int b_index);
void				assign_index(t_stack **a);
void				silent_rra(t_stack **a);
void				find_min_value_and_pos(t_stack *stack, int *min_value,
						int *min_pos);
void				optimized_initial_push(t_stack **a, t_stack **b);
void				fill_array_from_stack(t_stack *stack, int *arr, int size);
void				bubble_sort_array(int *arr, int size);
void				assign_index_to_stack(t_stack *stack, int *arr, int size);
void				free_stack(t_stack **stack);
size_t				ft_strlen(const char *s);
char				*ft_strdup(const char *s);
char				*ft_strjoin(char *s1, char *s2);
char				*ft_strchr(const char *s, int c);
char				*ft_strcpy(char *dest, const char *src);
char				*get_next_line(int fd);
int					read_join(int fd, char **loot);
char				*extract_line(char *loot);
char				*clear_loot(char *loot);
int					ft_strcmp(char *s1, char *s2);
void				validate_input_bonus(int argc, char **argv, t_stack **a);
void				silent_sa(t_stack *a);
void				silent_sb(t_stack *b);
void				silent_ss(t_stack *a, t_stack *b);
void				silent_rr(t_stack **a, t_stack **b);
void				silent_ra(t_stack **a);
void				silent_rrr(t_stack **a, t_stack **b);
void				silent_pb(t_stack **a, t_stack **b);

#endif