/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 16:57:51 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/08/21 16:57:52 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

#include <pthread.h>

void	init_struct(t_tracer *tracer)
{
	tracer->mlx_ptr = NULL;
	tracer->win_ptr = NULL;

	tracer->camera_position = NULL;
	tracer->camera_rotation = NULL;

	tracer->shapes = NULL;
	tracer->lights = NULL;

	tracer->d = NULL;

	tracer->degrees_x = 0;
	tracer->degrees_y = 0;
}


int		x_exit(void *param)
{
	param = NULL;
	exit(1);
	return (0);
}

int choose_key(int key, t_tracer *tracer)
{
	if (key == ESC)
		x_exit(tracer);
	else if (key == ARROW_UP)
		tracer->camera_position->z += 1;
	else if (key == ARROW_DOWN)
		tracer->camera_position->z -= 1;
	else if (key == ARROW_LEFT)
		tracer->camera_position->x -= 1;
	else if (key == ARROW_RIGHT)
		tracer->camera_position->x += 1;
	else if (key == NUM_8)
		tracer->camera_position->y += 1;
	else if (key == NUM_2)
		tracer->camera_position->y -= 1;
	else if (key == KEY_W)
		tracer->degrees_x -= 10;
	else if (key == KEY_S)
		tracer->degrees_x += 10;
	else if (key == KEY_A)
		tracer->degrees_y -= 10;
	else if (key == KEY_D)
		tracer->degrees_y += 10;

	// render(tracer);
	start_threads(tracer);

	return (0);
}

double	deg_to_rad(double degrees)
{
	return (degrees * M_PI / 180);
}

void	put_pixel(t_tracer *tracer, int x, int y, int color)
{
	int screen_x;
	int screen_y;

	screen_x = WIDTH / 2 + x;
	screen_y = HEIGHT / 2 - y - 1;

	if (screen_x < 0 || screen_x >= WIDTH || screen_y < 0 || screen_y >= HEIGHT)
		return ;

	// mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, screen_x, screen_y, color);
		tracer->image[screen_x + screen_y * HEIGHT] = color;
}

t_point *canvas_to_viewport(int x, int y)
{
	t_point *view_point;
	double	a;
	double	b;

	a = x * VIEWPORT_SIZE / (double)WIDTH;
	b = y * VIEWPORT_SIZE / (double)HEIGHT;
	view_point = create_point(a, b, PROJECTION_PLANE_Z);
	return (view_point);
}

t_point *reflect_ray(t_point *vec1, t_point *vec2)
{
	t_point *temp;
	t_point *sub;

	temp = mult_k_vec(2 * dot_product(vec1, vec2), vec2);
	sub = subtract_points(temp, vec1);
	free(temp);
	return (sub);
}

t_closest	*init_closest(void)
{
	t_closest	*closest;

	closest = (t_closest *)malloc(sizeof(t_closest));
	closest->closest_v = INFINIT;
	closest->closest_shape = NULL;
	return (closest);
}

t_closest	*closest_intersection(t_tracer *tracer, t_point *origin, t_point *d, double *min_max)
{
	t_shape		*current_shape;
	double		*ts;
	t_closest	*closest_params;

	ts = NULL;
	closest_params = init_closest();
	current_shape = tracer->shapes;
	while (current_shape != NULL)
	{
		if (ts != NULL)
			free(ts);
		if (current_shape->type == SPHERE)
			ts = intersect_ray_sphere(tracer, origin, d, current_shape);
		else if (current_shape->type == PLANE)
			ts = intersect_ray_plane(tracer, origin, d, current_shape);
		else if (current_shape->type == CYLINDER)
			ts = intersect_ray_cylinder(tracer, origin, d, current_shape);
		else if (current_shape->type == CONE)
			ts = intersect_ray_cone(tracer, origin, d, current_shape);

		if (ts[0] < closest_params->closest_v && min_max[0] < ts[0] && ts[0] < min_max[1])
		{
			closest_params->closest_v = ts[0];
			closest_params->closest_shape = current_shape;
		}
		if (ts[1] < closest_params->closest_v && min_max[0] < ts[1] && ts[1] < min_max[1])
		{
			closest_params->closest_v = ts[1];
			closest_params->closest_shape = current_shape;
		}
		current_shape = current_shape->next;
	}
	if (ts != NULL)
		free(ts);
	if (closest_params->closest_shape != NULL)
		return (closest_params);
	free(closest_params);
	return (NULL);
}

double	compute_lighting(t_tracer *tracer, t_point *point, t_point *normal,
													t_point *view, double specular)
{
	double		intensity;
	double		length_n;
	t_light		*current;
	double		n_dot_l;
	t_point		*vec_l;
	// for specular
	double		length_v;
	double min_max[2];
	int need_free = 0;



	length_n = length_vec(normal);
	length_v = length_vec(view);

	intensity = 0;
	current = tracer->lights;
	while (current != NULL)
	{
		if (current->type == AMBIENT)
			intensity += current->intensity;
		else
		{
			double max;

			if (current->type == POINT)
			{
				vec_l = subtract_points(current->position, point);
				need_free = 1;
				max = 1.0;
			}
			else // DIRECTIONAL
			{
				vec_l = current->position;
				max = INFINIT;
			}

			// Shadow check for STEP 4
			min_max[0] = EPSILON;
			min_max[1] = max;
			t_closest *blocker = closest_intersection(tracer, point, vec_l, min_max);
			if (blocker != NULL)
			{
				free(blocker);
				free(vec_l);
				current = current->next;
				continue;
			}

			// Diffuse reflection
			n_dot_l = dot_product(normal, vec_l);
			if (n_dot_l > 0)
				intensity += current->intensity * n_dot_l / (length_n * length_vec(vec_l));

			// Specular reflection
			if (specular >= 0)
			{
				t_point *vec_r;
				double	r_dot_v;

				vec_r = reflect_ray(vec_l, normal);
				r_dot_v = dot_product(vec_r, view);

				if (r_dot_v > 0)					
					intensity += current->intensity * pow(r_dot_v / (length_vec(vec_r) * length_v), specular);
				free(vec_r);
			}
		}
		free(vec_l);
		current = current->next;
	}
	return (intensity);
}

int		mult_k_color(double k, int color)
{
	int r;
	int g;
	int b;

	r = (0xFF0000 & color) >> 16;
	g = (0x00FF00 & color) >> 8;
	b = (0x0000FF & color);

	r *= k;
	g *= k;
	b *= k;

	// Clamps a color to the canonical color range.
	r = fmin(255, fmax(0, r));
	g = fmin(255, fmax(0, g));
	b = fmin(255, fmax(0, b));

	return ((r << 16) | (g << 8) | (b));
}

int		normal_light(t_tracer *tracer, t_closest *closest_params, t_point *point, t_point *origin)
{
	t_point		*normal;
	t_point		*view;
	double		lighting;
	int			local_color;

	if (closest_params->closest_shape->type == SPHERE)
		normal = sphere_normal(closest_params, point);
	else if (closest_params->closest_shape->type == PLANE)
		normal = plane_normal(closest_params);
	else if (closest_params->closest_shape->type == CYLINDER)
		normal = cylinder_normal(closest_params, point, origin, tracer->d);
	else if (closest_params->closest_shape->type == CONE)
		normal = cone_normal(closest_params, point, origin, tracer->d);
	view = mult_k_vec(-1.0, tracer->d);
	lighting = compute_lighting(tracer, point, normal, view,
							closest_params->closest_shape->specular);
	local_color = mult_k_color(lighting, closest_params->closest_shape->color);
	free(normal);
	free(view);
	return (local_color);
}

int		trace_ray(t_tracer *tracer, t_point *origin, double t_min, double t_max)
{
	t_closest	*closest_params;
	t_point		*point;
	int			local_color;
	double		min_max[2];

	min_max[0] = t_min;
	min_max[1] = t_max;
	closest_params = closest_intersection(tracer, origin, tracer->d, min_max);
	if (closest_params == NULL)
		return (BACKGROUND);
	t_point *temp;
	temp = mult_k_vec(closest_params->closest_v, tracer->d);
	point = add_points(origin, temp);
	free(temp);
	local_color = normal_light(tracer, closest_params, point, origin);
	free(closest_params);
	free(point);
	return (local_color);
}

void	render(t_tracer *tracer)
{
	int x;
	int y;
	int color;

	x = -WIDTH / 2;
	while (x < WIDTH / 2)
	{
		y = tracer->start;
		while (y < tracer->start + HEIGHT / THREADS)
		// y = -HEIGHT / 2;
		// while (y < HEIGHT / 2)
		{
			// t_point *direction = canvas_to_viewport(x, y);
			tracer->d = canvas_to_viewport(x, y);
			rotation_x(tracer);
			// rotation_y(tracer);
			tracer->d = mult_vec_matrix(tracer->d, tracer->camera_rotation);
			color = trace_ray(tracer, tracer->camera_position, 1.0, INFINIT);
			put_pixel(tracer, x, y, color);
			free(tracer->d);
			y++;
		}
		x++;
	}
}

void	start_threads(t_tracer *tracer)
{
	pthread_t	threads[THREADS];
	t_tracer	tracers[THREADS];
	int i;
	int temp_start;

	i = 0;
	temp_start = -HEIGHT / 2;
	while (i < THREADS)
	{
		tracers[i] = *tracer;
		tracers[i].start = temp_start;
		pthread_create(&threads[i], NULL, (void *)render, &tracers[i]);
		temp_start += HEIGHT / THREADS;
		i++;
	}
	i = 0;
	while (i < THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	mlx_put_image_to_window(tracer->mlx_ptr, tracer->win_ptr, tracer->img_ptr, 0, 0);
}

// compilation without FLAGS
int main(int argc, char *argv[])
{
	t_tracer	*tracer;

	if (argc != 2)
		print_error("Usage: ./RTv1 scene.txt");
	if (ft_strlen(argv[1]) < 5 || ft_strrchr(argv[1], '.') == NULL)
		print_error("Wrong file");
	char *extension = ft_strrchr(argv[1], '.');
	if (ft_strequ(extension, ".txt") == 0)
		print_error("Wrong extension of file");

	tracer = (t_tracer*)malloc(sizeof(t_tracer));
	init_struct(tracer);
	tracer->mlx_ptr = mlx_init();
	tracer->win_ptr = mlx_new_window(tracer->mlx_ptr, WIDTH, HEIGHT, "TRACER");
	tracer->img_ptr = mlx_new_image(tracer->mlx_ptr, WIDTH, HEIGHT);
	tracer->image = (int *)mlx_get_data_addr(tracer->img_ptr,
		&tracer->bits_per_pixel, &tracer->size_line, &tracer->endian);

	// ====================== READ DATA ========================
	// tracer->camera_position = create_point(0, 0, 0);

	read_data(tracer, argv[1]);
	if (tracer->camera_position == NULL)
		print_error("Camera must be set");
	init_rotation(tracer);

	// print_list_shapes(tracer->shapes);
	start_threads(tracer);
	// render(tracer);

	mlx_hook(tracer->win_ptr, 2, 5, choose_key, tracer);
	mlx_hook(tracer->win_ptr, 17, 1L << 17, x_exit, 0);
	mlx_loop(tracer->mlx_ptr);

	return (0);
}