/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssergiu <ssergiu@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 02:13:15 by ssergiu           #+#    #+#             */
/*   Updated: 2022/11/07 20:15:31 by ssergiu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include <fcntl.h>
# include "libft.h"
# include "get_next_line.h"
# include <stdio.h>
# include <sys/errno.h>
# include <string.h>
# include <math.h>
# define WIDTH 1920
# define HEIGHT 1080

typedef struct coords
{
	double	x;	
	double	y;
}		t_coords;

typedef struct data 
{
	mlx_t *mlx;
	mlx_image_t *img;
	t_coords *coordsA;
	t_coords *coordsB;
	t_coords *coordsC;
	t_coords *coordsD;
}		t_data;

char	*read_map(char *map);

#endif
