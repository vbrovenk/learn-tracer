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

# define WIDTH 320
# define HEIGHT 320
# define BACKGROUND 0xFFFFFF

# define MIN_T 1
# define INFINIT 2147483647
# define EPSILON 0.001
# define RECURSION_DEPTH 0

# define VIEWPORT_SIZE 1
# define PROJECTION_PLANE_Z 1
# define FOV 60

# define THREADS 4

/*
** For light
*/
# define AMBIENT 11
# define POINT 12
# define DIRECTIONAL 13


/*
** Ubuntu buttons
*/
# define ARROW_UP 65362
# define ARROW_DOWN 65364
# define ARROW_LEFT 65361
# define ARROW_RIGHT 65363
# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100

# define NUM_8 65431
# define NUM_2 65433

# define ESC 65307

typedef struct s_point
{
	double x;
	double y;
	double z;

	int color;
}				t_point;

typedef	struct	s_m3x3
{
	double	matrix[3][3];
}				t_m3x3;

typedef	struct s_sphere
{
	t_point	*center;
	double	radius;
	int		color;

	double	specular;
	double	reflective;

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
	t_m3x3 *camera_rotation;
	
	t_sphere	*spheres;
	t_light		*lights;

	int degrees_x;
	int degrees_y;

	// for threads;
	int start;
}				t_tracer;

typedef	struct s_closest
{
	double		closest_t;
	t_sphere	*closest_sphere;
}				t_closest;


/*
** main.c
*/
void	render(t_tracer *tracer);
void	start_threads(t_tracer *tracer);

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
t_point *mult_vec_matrix(t_point *vec, t_m3x3 *m);

/*
** rotation.c
*/
void	init_rotation(t_tracer *tracer);
void	rotation_x(t_tracer *tracer);

#endif