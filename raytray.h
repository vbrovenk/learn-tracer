



#ifndef RAYTRAY_H
#define RAYTRAY_H

#include <math.h>
#include <stdio.h>
#include "libft.h"
#include <mlx.h>

#define WIDTH 512
#define HEIGHT 512

#define ESC 65307

typedef struct s_point
{
	double x;
	double y;
	double z;

	int color;
}				t_point;


typedef struct s_tracer
{
	void *mlx_ptr;
	void *win_ptr;
	void *img_ptr;
	int *image;

	int bits_per_pixel;
	int	size_line;
	int endian;

}				t_tracer;


#endif