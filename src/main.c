/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssergiu <ssergiu@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 06:57:33 by ssergiu           #+#    #+#             */
/*   Updated: 2022/11/09 10:11:22 by ssergiu          ###   ########.fr       */
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
	double x;
	double y;

	//bresenham algorithm
	x = pointA->x;
	y = pointA->y;
	dX = pointB->x - x;
	dY = pointB->y - y;
	p = 2 * dY- dX;
	printf("x is %f\n", x);
	printf("y is %f\n", y);
	printf("pointB->x is %f\n", pointB->x);
	printf("pointB->y is %f\n", pointB->y);
	while (x < pointB->x)
	{
		if (p >= 0)
		{
			mlx_put_pixel(y_img, x++, y++, 0xFAFABB);
			p = p + (2 * dY) - (2 * dX);
		}
		else if (p < 0)
		{
			mlx_put_pixel(y_img, x++, y, 0xFAFABB);
			p = p + (2 * dY);
		}
	}
}

void	rotate_line(t_coords *pointA, t_coords *pointB, mlx_image_t *img)
{
	double dX;
	double dY;
	double radians;

	radians = 1 * M_PI/180;
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
	static mlx_image_t *image1 = NULL;
	static mlx_image_t *image2 = NULL;
	static mlx_image_t *image3 = NULL;
	static mlx_image_t *image4 = NULL;
	static mlx_image_t *image5 = NULL;
	int x;

	data = param;

	x = 1;
	if (keydata.key == MLX_KEY_Z && keydata.action == MLX_PRESS)
	{
		puts("HELLO");
	}
	if (keydata.key == MLX_KEY_X && keydata.action == MLX_PRESS)
	{
//		draw_line(data->coordsA, data->coordsC, data->img);
//		printf("-- x %f and Y %f\n", data->coordsB->x, data->coordsB->y);
		rotate_line(data->coordsA, data->coordsB, data->img);
		mlx_delete_image(data->mlx, image1);
		mlx_delete_image(data->mlx, image2);
		mlx_delete_image(data->mlx, image3);
		mlx_delete_image(data->mlx, image4);
		mlx_delete_image(data->mlx, image5);
		image1 = mlx_put_string(data->mlx, ft_itoa(data->coordsB->x), 20, 20);
		image2 = mlx_put_string(data->mlx, ft_itoa(data->coordsB->y), 60, 20);
		image3 = mlx_put_string(data->mlx, ft_itoa(data->coordsB->x - data->coordsA->x)
				, 120, 20);
		image4 = mlx_put_string(data->mlx, ft_itoa(data->coordsB->y - data->coordsA->y)
				, 160, 20);
		image5 = mlx_put_string(data->mlx, ft_itoa(2 * ((data->coordsB->x - 
						data->coordsA->x) - (data->coordsB->y - data->coordsA->y)))
						, 20, 40);
	}

	// If we RELEASE the 'K' key, print "World".
	if (keydata.key == MLX_KEY_K && keydata.action == MLX_PRESS)
		ft_bzero(data->img->pixels, data->img->width * data->img->height * 4);

	// If we HOLD the 'L' key, print "!".
	if (keydata.key == MLX_KEY_L && keydata.action == MLX_PRESS)
	{
		printf("L\n");
		draw_line(data->coordsA, data->coordsC, data->img);
	}
	if (keydata.key == MLX_KEY_M && keydata.action == MLX_PRESS)
	{
		printf("D\n");
		data->coordsC->x = 300;
		data->coordsC->y = 600;
		draw_line(data->coordsA, data->coordsC, data->img);
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
	pointA->x=10;
	pointA->y=10;

	pointB->x=100;
	pointB->y=10;

	pointC->x=350;
	pointC->y=459;

	data = (t_data *)malloc(sizeof(t_data));
	data->coordsA = pointA;
	data->coordsB = pointB;
	data->coordsC = pointC;
	mlx_set_setting(MLX_MAXIMIZED, true);
	data->mlx = mlx_init(WIDTH, HEIGHT, "fdf", true);
	if (!data->mlx)
		return(1);
	data->img = mlx_new_image(data->mlx, 1920, 1080);
	mlx_image_to_window(data->mlx, data->img, 1920, 1080);
	/* Do stuff */
	mlx_key_hook(data->mlx, &my_keyhook, data);
	mlx_loop(data->mlx);
	mlx_terminate(data->mlx);
	return (EXIT_SUCCESS);

}
