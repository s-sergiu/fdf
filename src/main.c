/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssergiu <ssergiu@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 06:57:33 by ssergiu           #+#    #+#             */
/*   Updated: 2022/11/04 05:58:34 by ssergiu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	error_handling(char errnum)
{
	if (errnum == 1)
		write(2, "Invalid map name..\n", 19);
	else
		printf("%s\n", strerror(errnum));
}

int	is_valid_map_name(char *filename)
{
	int	len;
	int	file;

	len = ft_strlen(filename);
	file = open(filename, O_RDONLY);
	if (file < 0)
	{
		error_handling(errno);
		return (0);
	}
	close(file);
	if (!ft_strncmp(filename + (len - 4), ".fdf\0", 5))
		return (1);
	error_handling(1);
	return (0);
}

char	*read_map(char *map)
{
	int		file;
	int		read_bytes;
	int		total_bytes;
	char	*buffer;
	char	*map_b;

	file = open(map, O_RDONLY);
	buffer = malloc(1);
	read_bytes = read(file, buffer, 4000);
	total_bytes = 0;
	total_bytes += read_bytes;
	while (read_bytes != 0)
	{
		read_bytes = read(file, buffer, 4000);
		total_bytes += read_bytes;
	}
	close(file);
	map_b = (char *)malloc(sizeof(char) * total_bytes);
	file = open(map, O_RDONLY);
	read(file, map_b, total_bytes);
	return(map_b);
}

#include "../lib/MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#define WIDTH 100
#define HEIGHT 100
#include <math.h>

mlx_image_t	*y_img;

void	draw_line(t_coords pointA, t_coords pointB, mlx_image_t* y_img)
{
	int dX;
	int	dY;
	int pD;
	int p;

	//bresenheim algorithm
	while (pointA.x < pointB.x)
	{
		dX = pointB.x - pointA.x;
		dY = pointB.y - pointA.y;
		p = (2 * dY)- dX;
		if (p >= 0)
		{
			pD = p + (2 * dY) - (2 * dX);
			mlx_put_pixel(y_img, pointA.x++, pointA.y++, 0xFFFFFF);
		}
		else if (p < 0)
		{
			pD = p + (2 * dY);
			mlx_put_pixel(y_img, pointA.x++, pointA.y, 0xFFFFFF);
		}
	}
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_coords pointA;
	t_coords pointB;

	pointA.x = 100;
	pointA.y = 240;

	pointB.x = 800;
	pointB.y = 440;
	int x;
	int y;

	x = 10;
	y = 11;
	// If we PRESS the 'J' key, print "Hello".
	if (keydata.key == MLX_KEY_J && keydata.action == MLX_PRESS)
	{
		draw_line(pointA, pointB, param);
		mlx_put_pixel(param, x, y, 0xFFFFFF);
	}

	// If we RELEASE the 'K' key, print "World".
	if (keydata.key == MLX_KEY_K && keydata.action == MLX_PRESS)
		pointB.x++;

	// If we HOLD the 'L' key, print "!".
	if (keydata.key == MLX_KEY_L && keydata.action == MLX_PRESS)
		pointB.y++;
}

int32_t	main(void)
{
	t_coords pointA;
	t_coords pointB;

	pointA.x = 20;
	pointA.y = 0;

	pointB.x = 90;
	pointB.y = 90;

	mlx_set_setting(MLX_MAXIMIZED, true);
	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!mlx)
		return(1);

	/* Do stuff */
	mlx_image_t* y_img = mlx_new_image(mlx, 1000, 1000);
	mlx_image_to_window(mlx, y_img, pointA.x, pointA.y);
	draw_line(pointA, pointB, y_img);
	mlx_key_hook(mlx, &my_keyhook, y_img);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);

}
