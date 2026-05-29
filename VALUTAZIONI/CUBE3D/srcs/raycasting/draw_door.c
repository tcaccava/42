/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:37:14 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/05/20 14:37:16 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

t_ray	*copy_ray(t_ray *old)
{
	t_ray	*copy;

	copy = calloc(1, sizeof(t_ray));
	copy->x = old->x;
	copy->hitpoint_x = old->hitpoint_x;
	copy->hitpoint_y = old->hitpoint_y;
	copy->ray_y = old->ray_y;
	copy->ray_x = old->ray_x;
	copy->dist_x = old->dist_x;
	copy->dist_y = old->dist_y;
	copy->side_dist_x = old->side_dist_x;
	copy->side_dist_y = old->side_dist_y;
	copy->ray_dir_x = old->ray_dir_x;
	copy->ray_dir_y = old->ray_dir_y;
	copy->side = old->side;
	copy->identifier = old->identifier;
	return (copy);
}

void	add_list(t_list **rays, t_ray *ray, int x, int y)
{
	t_list	*node;
	t_ray	*ray_copy;

	ray_copy = copy_ray(ray);
	node = ft_lstnew(ray_copy);
	ray_copy->x_map = x;
	ray_copy->y_map = y;
	ft_lstadd_front(rays, node);
}

void	draw_sprite(t_list **rays, t_data *data)
{
	t_list	**copy;
	t_list	*temp;
	int		x;
	int		y;

	copy = rays;
	while (*rays)
	{
		temp = *rays;
		x = ((t_ray *)(*rays)->content)->x_map;
		y = ((t_ray *)(*rays)->content)->y_map;
		draw_wall(x, y, (t_ray *)((*rays)->content), data);
		free((*rays)->content);
		*rays = (*rays)->next;
		free(temp);
	}
	free(copy);
}
