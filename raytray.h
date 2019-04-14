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
# define BACKGROUND 0x000000

# define MIN_T 1
# define INFINIT 2147483647
# define EPSILON 0.001
# define RECURSION_DEPTH 0

# define VIEWPORT_SIZE 1
# define PROJECTION_PLANE_Z 1
# define FOV 60

# define THREADS 1

/*
** For light
*/
# define AMBIENT 11
# define POINT 12
# define DIRECTIONAL 13

/*
** shapes
*/

# define PLANE 20
# define SPHERE 21

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

/*
** MAC buttons
*/
// # define ARROW_UP 126
// # define ARROW_DOWN 125
// # define ARROW_LEFT 123
// # define ARROW_RIGHT 124
// # define KEY_W 13
// # define KEY_S 1
// # define KEY_A 0
// # define KEY_D 2

// # define NUM_8 91
// # define NUM_2 84

// # define ESC 53

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

typedef struct s_shape
{
	int		type;

	t_point	*center;
	double	radius;
	int		color;

	double	specular;
	double	reflective;

	struct s_shape *next;
}				t_shape;


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

	t_shape		*shapes;

	t_light		*lights;

	int degrees_x;
	int degrees_y;

	// for threads;
	int start;
}				t_tracer;

typedef	struct s_closest
{
	double		closest_v;
	t_shape	*closest_shape;
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
void	rotation_y(t_tracer *tracer);

/*
** create_add.c
*/
t_shape *create_shape(int type, t_point *center, double radius, int color,
										double specular, double reflective);
t_sphere *init_sphere(t_point *center, double radius, int color,
										double specular, double reflective);
t_light	*create_light(t_point *position, int type, double intensity);
void	add_shape_to_list(t_shape **head, t_shape *shape);
void	add_sphere_to_list(t_sphere **head, t_sphere *sphere);
void	add_light_to_list(t_light **head, t_light *light);

/*
** print.c
*/
void	info_about_point(t_point *point);
void	info_about_sphere(t_sphere *sphere);
void	info_about_light(t_light *light);
void	print_list_spheres(t_sphere *head);
void	print_list_lights(t_light *head);
void	info_about_matrix4x4(double (*matrix)[4]);


#endif