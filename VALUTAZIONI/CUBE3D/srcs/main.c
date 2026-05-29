/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:34:37 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/05/23 16:34:39 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static int	create_img(t_data *data);
static void	set_pointers(t_data *data, t_map *map, t_player *player, char **av);
static void	parser(t_data *data, t_map *map, t_player *player, char **av);
static void	mlx_hooks(t_data *data);

int	main(int ac, char **av)
{
	static t_data	data;
	static t_map	map;
	static t_player	player;
	static t_camera	camera;

	if (ac != 2)
		return (0);
	parser(&data, &map, &player, av);
	data.camera = &camera;
	create_img(&data);
	setup_direction(&player);
	set_texture(&data);
	draw(&data);
	mlx_hooks(&data);
	mlx_loop(data.mlx_ptr);
	free_textures(&data);
	return (0);
}

static void	mlx_hooks(t_data *data)
{
	mlx_hook(data->win_ptr, 2, 1L << 0, key_hook, data);
	mlx_mouse_hook(data->win_ptr, mouse_hook, data);
	mlx_hook(data->win_ptr, 17, 0L, free_exit, data);
}

static void	parser(t_data *data, t_map *map, t_player *player, char **av)
{
	set_pointers(data, map, player, av);
	extract_map_only(data);
	get_map_infos(data);
	is_map_closed(data);
	is_map_valid(data);
	rgb_converter(data, 'F');
	rgb_converter(data, 'C');
	data->mlx_ptr = mlx_init();
}

static void	set_pointers(t_data *data, t_map *map, t_player *player, char **av)
{
	check_format(av[1]);
	data->map_ptr = open(av[1], O_RDONLY);
	if (data->map_ptr == -1)
	{
		printf("Error: couldn't open map\n");
		exit(1);
	}
	data->player = player;
	data->map = map;
	data->map->map = fill_map(data);
	data->map->door_txt_path[0] = "./srcs/sprites/cube3dtex/xpm/door1.xpm";
	data->map->door_txt_path[1] = "./srcs/sprites/cube3dtex/xpm/door2.xpm";
	data->map->door_txt_path[2] = "./srcs/sprites/cube3dtex/xpm/door3.xpm";
	data->map->door_txt_path[3] = "./srcs/sprites/cube3dtex/xpm/door4.xpm";
	data->map->is_map_valid = 1;
}

static int	create_img(t_data *data)
{
	if (!data->mlx_ptr)
		return (0);
	data->win_ptr = mlx_new_window(data->mlx_ptr, WIDTH, HEIGHT, "CUB3D");
	if (!data->win_ptr)
		return (0);
	data->img = mlx_new_image(data->mlx_ptr, WIDTH, HEIGHT);
	if (!data->img)
		return (0);
	data->img_addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	return (1);
}
