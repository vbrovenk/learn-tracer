/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytray.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 16:57:51 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/08/21 16:57:52 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYTRAY_H
#define RAYTRAY_H

#include <math.h>
#include <stdio.h>
#include "libft.h"
#include <mlx.h>

#define WIDTH 512
#define HEIGHT 512
#define BACKGROUND 0xFFFFFF

#define MIN_T 1
#define INFINIT 2147483647

#define VIEWPORT_SIZE 1
#define PROJECTION_PLANE_Z 1
#define FOV 60


/*
** For light
*/
#define AMBIENT 11
#define POINT 12
#define DIRECTIONAL 13

#define ESC 65307

typedef struct s_point
{
	double x;
	double y;
	double z;

	int color;
}				t_point;

typedef	struct	s_m4x4
{
	double	matrix[4][4];
}				t_m4x4;

typedef	struct s_sphere
{
	t_point	*center;
	double	radius;
	int		color;

	double	specular;
	struct s_sphere *next;
}				t_sphere;

typedef struct s_light
{
	t_point	*position;
	int		type;
	double	intensity;
	
	struct s_light *next;
}				t_light;

typedef struct s_tracer
{
	void *mlx_ptr;
	void *win_ptr;
	void *img_ptr;
	int *image;

	int bits_per_pixel;
	int	size_line;
	int endian;

	// for ray-tracer
	t_point *camera_position;

}				t_tracer;


/*
** actions_with_points.c
*/
t_point *create_point(double x, double y, double z);
t_point *subtract_points(t_point *end, t_point *start);
t_point *add_points(t_point *end, t_point *start);
double	dot_product(t_point *p1, t_point *p2);
t_point *normalize(t_point *p);
t_point *mult_k_vec(double k, t_point *vec);
double	length_vec(t_point *vec);

/*
** actions_with_matrix.c
*/
t_point *mult_vec_matrix(t_point *vec, double (*m)[4]);


#endif