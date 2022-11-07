/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssergiu <ssergiu@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 06:57:33 by ssergiu           #+#    #+#             */
/*   Updated: 2022/11/07 20:33:58 by ssergiu          ###   ########.fr       */
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

void	draw_line(t_coords *pointA, t_coords *pointB, mlx_image_t* y_img)
{
	double dX;
	double dY;
	double p;
	double testx;
	double testy;

	//bresenham algorithm
	testx = pointA->x;
	testy = pointA->y;
	printf("draw - pointB-x %f\n", pointB->x);
	printf("draw - pointB-y %f\n", pointB->y);
	printf("draw - pointA-x %f\n", pointA->x);
	printf("draw - pointA-y %f\n", pointA->y);
	printf("draw - test-x %f\n", testx);
	printf("draw - test-y %f\n", testy);
		dX = pointB->x - testx;
		dY = pointB->y - testy;
		p = (2 * dY)- dX;
	while (testx < pointB->x)
	{
		if (p >= 0)
		{
			mlx_put_pixel(y_img, testx++, testy++, 0xFAFABB);
			p = p + (2 * dY) - (2 * dX);
		}
		else if (p < 0)
		{
			mlx_put_pixel(y_img, testx++, testy, 0xFAFABB);
			p = p + (2 * dY);
		}
	}
}

mlx_image_t	*y_img;
void	rotate_line(t_coords *pointA, t_coords *pointB, mlx_image_t *img)
{
	double dX;
	double dY;
	double radians;

	radians = 5 * M_PI/180;
	dX = ( cos(radians) * (pointB->x - pointA->x)) - ( sin(radians) * (pointB->y -
				pointA->y)) + pointA->x;
	dY = ( sin(radians) * (pointB->x - pointA->x)) + ( cos(radians) * (pointB->y -
				pointA->y)) + pointA->y;

	pointB->x = dX;
	pointB->y = dY;
	ft_bzero(img->pixels, img->width * img->height * 4);
	draw_line(pointA, pointB, img);
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_data *data;
	int x;

	data = param;

	x = 1;
	if (keydata.key == MLX_KEY_X && keydata.action == MLX_PRESS)
	{
//		draw_line(data->coordsA, data->coordsC, data->img);
//		printf("-- x %f and Y %f\n", data->coordsB->x, data->coordsB->y);
		mlx_put_string(data->mlx, "test", 10, 20);
		rotate_line(data->coordsA, data->coordsB, data->img);
	}

	// If we RELEASE the 'K' key, print "World".
	if (keydata.key == MLX_KEY_K && keydata.action == MLX_PRESS)
		ft_bzero(data->img->pixels, data->img->width * data->img->height * 4);

	// If we HOLD the 'L' key, print "!".
	if (keydata.key == MLX_KEY_L && keydata.action == MLX_PRESS)
	{
		printf("L\n");
		draw_line(data->coordsA, data->coordsB, data->img);
	}

}

int32_t	main(void)
{
	t_coords	*pointA;
	t_coords	*pointB;
	t_coords	*pointC;
	t_data		*data;

	pointA = (t_coords *)malloc(sizeof(t_coords));
	pointB = (t_coords *)malloc(sizeof(t_coords));
	pointC = (t_coords *)malloc(sizeof(t_coords));
	pointA->x=200;
	pointA->y=200;
	pointB->x=500;
	pointB->y=200;
	pointC->x=412;
	pointC->y=340;

	data = (t_data *)malloc(sizeof(t_data));
	data->coordsA = pointA;
	data->coordsB = pointB;
	data->coordsC = pointC;
	mlx_set_setting(MLX_MAXIMIZED, true);
	data->mlx = mlx_init(WIDTH, HEIGHT, "fdf", true);
	if (!data->mlx)
		return(1);
	data->img = mlx_new_image(data->mlx, 1920, 1080);
	mlx_image_to_window(data->mlx, data->img, 500, 500);
	/* Do stuff */
	mlx_key_hook(data->mlx, &my_keyhook, data);
	mlx_loop(data->mlx);
	mlx_terminate(data->mlx);
	return (EXIT_SUCCESS);

}
