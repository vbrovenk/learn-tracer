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
# define RAYTRAY_H

# include <math.h>
# include "libft.h"
# include <mlx.h>
# include <fcntl.h>
# include <pthread.h>

# define WIDTH 520
# define HEIGHT 520
# define BACKGROUND 0x000010

# define INFINIT 2147483647
# define EPSILON 0.001

# define VIEWPORT_SIZE 1
# define PROJECTION_PLANE_Z 1

# define THREADS 8

/*
** For light
*/
# define POINT 12

/*
** shapes
*/
# define PLANE 20
# define SPHERE 21
# define CYLINDER 22
# define CONE 23

# define ARROW_UP 126
# define ARROW_DOWN 125
# define ARROW_LEFT 123
# define ARROW_RIGHT 124
# define KEY_W 13
# define KEY_S 1
# define KEY_A 0
# define KEY_D 2
# define NUM_8 91
# define NUM_2 84
# define ESC 53

typedef struct		s_point
{
	double			x;
	double			y;
	double			z;

	int				color;

}					t_point;

typedef struct		s_shape
{
	int				type;

	t_point			*center;
	double			radius;
	int				color;
	double			height_cylinder;

	double			angle;
	double			height_cone1;
	double			height_cone2;

	t_point			*dir;
	double			specular;
	struct s_shape	*next;

}					t_shape;

typedef struct		s_light
{
	t_point			*position;
	int				type;
	double			intensity;
	struct s_light	*next;

}					t_light;

typedef struct		s_specular
{
	t_light			*current_light;
	t_point			*vec_l;
	t_point			*view;
	double			intensity;

}					t_specular;

/*
**	s_tracer - main structure
**	d - direction of ray
*/
typedef struct		s_tracer
{
	void			*mlx_ptr;
	void			*win_ptr;
	void			*img_ptr;
	int				*image;

	int				bits_per_pixel;
	int				size_line;
	int				endian;
	t_point			*camera_position;

	t_point			*d;

	t_shape			*shapes;
	t_light			*lights;

	int				degrees_x;
	int				degrees_y;
	int				start;
}					t_tracer;

typedef struct		s_closest
{
	double			closest_v;
	t_shape			*closest_shape;

}					t_closest;

/*
** main.c
*/
void				start_threads(t_tracer *tracer);

/*
** actions_with_points.c
*/
t_point				*create_point(double x, double y, double z);
t_point				*subtract_points(t_point *end, t_point *start);
t_point				*add_points(t_point *end, t_point *start);
double				dot_product(t_point *p1, t_point *p2);
t_point				*mult_k_vec(double k, t_point *vec);

/*
** actions_with_matrix.c
*/
t_point				*normalize(t_point *p);
double				length_vec(t_point *vec);

/*
** rotation.c
*/
t_point				*rotation_x(t_point *old, int degrees);
t_point				*rotation_y(t_point *old, int degrees);

/*
** create_add.c
*/
t_shape				*create_shape(int type);
t_light				*create_light(t_point *position, int type,
													double intensity);
void				add_shape_to_list(t_shape **head, t_shape *shape);
void				add_light_to_list(t_light **head, t_light *light);

/*
** scan_data.c
*/
void				read_data(t_tracer *tracer, char *filename);
int					general_options(char *line, t_shape *shape);
void				read_direction(char *line, t_point *direction);

/*
** sphere.c
*/
double				*intersect_ray_sphere(t_point *origin,
										t_point *direction, t_shape *shape);
t_point				*sphere_normal(t_closest *closest_params, t_point *point);
void				read_sphere(t_tracer *tracer, int fd);

/*
** plane.c
*/
double				*intersect_ray_plane(t_point *origin,
										t_point *direction, t_shape *shape);
t_point				*plane_normal(t_closest *closest_params);
void				read_plane(t_tracer *tracer, int fd);

/*
** cylinder.c
*/
double				*intersect_ray_cylinder(t_point *origin,
										t_point *direction, t_shape *shape);
t_point				*cylinder_normal(t_closest *closest_params,
						t_point *point, t_point *origin, t_point *direction);
void				read_cylinder(t_tracer *tracer, int fd);

/*
** cone.c
*/
double				*intersect_ray_cone(t_point *origin, t_point *direction,
															t_shape *shape);
t_point				*cone_normal(t_closest *closest_params, t_point *point,
										t_point *origin, t_point *direction);

/*
** light.c
*/
double				compute_lighting(t_tracer *tracer, t_point *point,
											t_point *normal, double specular);

/*
** scan_light.c
*/
void				read_light(t_tracer *tracer, int fd);
int					read_color(char *line);
int					is_number(char *line);

/*
** intersection.c
*/
t_closest			*closest_intersection(t_tracer *tracer, t_point *origin,
												t_point *d, double *min_max);

/*
** camera.c
*/
void				read_camera(t_tracer *tracer, int fd);
void				read_position(char *line, t_point *point);
double				read_double(char *line);
int					count_splits(char **split);
void				clear_split(char **split);

/*
** canvas.c
*/
void				put_pixel(t_tracer *tracer, int x, int y, int color);
t_point				*canvas_to_viewport(int x, int y);
int					mult_k_color(double k, int color);

/*
** hooks.c
*/
t_tracer			*init_struct(void);
int					x_exit(void *param);
int					choose_key(int key, t_tracer *tracer);
void				print_error(char *msg);

#endif
