/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssergiu <ssergiu@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 06:57:33 by ssergiu           #+#    #+#             */
/*   Updated: 2022/11/06 19:23:31 by ssergiu          ###   ########.fr       */
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

mlx_image_t	*y_img;

void	draw_line(t_coords *pointA, t_coords *pointB, mlx_image_t* y_img)
{
	double dX;
	double dY;
	double pD;
	double p;
	double testx;
	double testy;

	//bresenham algorithm
	testx = pointA->x;
	testy = pointA->y;
	while (testx < testy)
	{
		dX = pointB->x - testx;
		dY = pointB->y - testy;
			
		printf(" pointB->x %p\n", &testx);
		printf("asdasda pointB->x %p\n", &pointB->x);

//		printf("these two assholes %f ,%f\n", dX, dY);
		p = (2 * dY)- dX;
		if (p >= 0)
		{
			pD = p + (2 * dY) - (2 * dX);
			mlx_put_pixel(y_img, testx++, testy++, 0xFAFABB);
		}
		else if (p < 0)
		{
			pD = p + (2 * dY);
			mlx_put_pixel(y_img, testx++, testy, 0xFAFABB);
		}
	}
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_data *data;

	data = param;

	if (keydata.key == MLX_KEY_X && keydata.action == MLX_PRESS)
	{

		printf("%p \n", data->img);
		draw_line(data->coordsC, data->coordsD, data->img);
		printf("put line\n");
	}
	// If we PRESS the 'J' key, print "Hello".
	if (keydata.key == MLX_KEY_J && keydata.action == MLX_PRESS)
	{
		data->coordsA->x = 100;
		data->coordsA->y = 100;
		puts("hello");
	}

	// If we RELEASE the 'K' key, print "World".
	if (keydata.key == MLX_KEY_K && keydata.action == MLX_PRESS)
	{
		printf("image width : %d\n image height : %d\n", data->img->width,
				data->img->height);
		ft_bzero(data->img->pixels, data->img->width * data->img->height * 4);
	}

	// If we HOLD the 'L' key, print "!".
	if (keydata.key == MLX_KEY_L && keydata.action == MLX_PRESS)
	{
		draw_line(data->coordsA, data->coordsB, data->img);
		printf("delete\n");
	}
}

int32_t	main(void)
{
	t_coords	*pointA;
	t_coords	*pointB;
	t_coords	*pointC;
	t_coords	*pointD;
	t_data		*data;

	pointA = (t_coords *)malloc(sizeof(t_coords));
	pointB = (t_coords *)malloc(sizeof(t_coords));
	pointA->x=200;
	pointA->y=200;
	pointB->x=400;
	pointB->y=200;
	pointC = (t_coords *)malloc(sizeof(t_coords));
	pointD = (t_coords *)malloc(sizeof(t_coords));
	pointC->x=300;
	pointC->y=300;
	pointD->x=500;
	pointD->y=500;

	data = (t_data *)malloc(sizeof(t_data));
	data->coordsA = pointA;
	data->coordsB = pointB;
	data->coordsC = pointC;
	data->coordsD = pointD;
	mlx_set_setting(MLX_MAXIMIZED, true);
	data->mlx = mlx_init(WIDTH, HEIGHT, "fdf", true);
	if (!data->mlx)
		return(1);
	data->img = mlx_new_image(data->mlx, 500, 500);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	/* Do stuff */
	printf("test %p\n", data->img);
	mlx_key_hook(data->mlx, &my_keyhook, data);
	mlx_loop(data->mlx);
	mlx_terminate(data->mlx);
	return (EXIT_SUCCESS);

}
