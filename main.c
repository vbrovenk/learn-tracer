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

	tracer->spheres = NULL;
	tracer->lights = NULL;

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

void	ft_swap(double *a, double *b)
{
	double temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void convert_to_conterclock(t_point *corners)
{
	for (int i = 0; i < 8; i++)
	{
		corners[i].y *= -1;
	}
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

	temp = mult_k_vec(2 * dot_product(vec1, vec2), vec2);
	return (subtract_points(temp, vec1));
}

double	*intersect_ray_sphere(t_tracer *tracer, t_point *origin, t_point *direction, t_shape *shape)
{
	t_point *oc;
	double	k1;
	double 	k2;
	double	k3;

	double discriminant;
	double t1;
	double t2;

	double	*res;

	res = malloc(sizeof(double) * 2);

	oc = subtract_points(origin, shape->center);

	k1 = dot_product(direction, direction);
	k2 = 2.0 * dot_product(oc, direction);
	k3 = dot_product(oc, oc) - shape->radius * shape->radius;

	discriminant = k2 * k2 - 4.0 * k1 * k3;
	if (discriminant < 0.0)
	{
		res[0] = INFINIT;
		res[1] = INFINIT;
		return (res);
	}

	t1 = (-k2 - sqrt(discriminant)) / (double)(2.0 * k1);
	t2 = (-k2 + sqrt(discriminant)) / (double)(2.0 * k1);
	res[0] = t1;
	res[1] = t2;
	return (res);
}


double	*intersect_ray_plane(t_tracer *tracer, t_point *origin, t_point *direction, t_shape *shape)
{
	double	*res;

	res = malloc(sizeof(double) * 2);
	t_point	*normal = create_point(0, 1, 0);
	res[0] = -dot_product(subtract_points(origin, shape->center), normal);
	res[1] = dot_product(direction, normal);
	if (res[1] != 0.0)
	{
		res[0] = res[0] / res[1];
		res[1] = INFINIT;
		if (length_vec(subtract_points(add_points(mult_k_vec(res[0], direction), origin), shape->center)) > shape->radius)
			res[0] = INFINIT;
		return (res);
	}
	res[0] = INFINIT;
	res[1] = INFINIT;
	return (res);
}

t_closest	*init_closest(void)
{
	t_closest	*closest;

	closest = (t_closest *)malloc(sizeof(t_closest));
	closest->closest_v = INFINIT;
	closest->closest_shape = NULL;
	return (closest);
}

t_closest	*closest_intersection(t_tracer *tracer, t_point *origin, t_point *direction,
											double min, double max)
{
	t_shape		*current_shape;
	double		*ts;
	t_closest	*closest_params;

	closest_params = init_closest();
	current_shape = tracer->shapes;
	while (current_shape != NULL)
	{
		if (current_shape->type == SPHERE)
			ts = intersect_ray_sphere(tracer, origin, direction, current_shape);
		else if (current_shape->type == PLANE)
			ts = intersect_ray_plane(tracer, origin, direction, current_shape);
		if (ts[0] < closest_params->closest_v && min < ts[0] && ts[0] < max)
		{
			closest_params->closest_v = ts[0];
			closest_params->closest_shape = current_shape;
		}
		if (ts[1] < closest_params->closest_v && min < ts[1] && ts[1] < max)
		{
			closest_params->closest_v = ts[1];
			closest_params->closest_shape = current_shape;	
		}
		current_shape = current_shape->next;
	}
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
			double t_max;

			if (current->type == POINT)
			{
				vec_l = subtract_points(current->position, point);
				t_max = 1.0;
			}
			else // DIRECTIONAL
			{
				vec_l = current->position;
				t_max = INFINIT;
			}

			// Shadow check for STEP 4
			t_closest *blocker = closest_intersection(tracer, point, vec_l, EPSILON, t_max);
			if (blocker != NULL)
			{
				current = current->next;
				continue;
			}

			// Diffuse reflection
			n_dot_l = dot_product(normal, vec_l);
			if (n_dot_l > 0)
				intensity += current->intensity * n_dot_l / (length_n * length_vec(vec_l));

			// Specular reflection
			if (specular != -1)
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

int		trace_ray(t_tracer *tracer, t_point *origin, t_point *direction,
									double t_min, double t_max, int depth)
{
	t_closest	*closest_params;

	closest_params = closest_intersection(tracer, origin, direction, t_min, t_max);

	if (closest_params == NULL)
		return (BACKGROUND);

	t_shape *closest_shape = closest_params->closest_shape;
	double	closest_v = closest_params->closest_v;

	// added in STEP 2
	t_point		*point;
	t_point		*normal;
	double		lighting;

	point = add_points(origin, mult_k_vec(closest_v, direction));

	// normal for sphere
	if (closest_shape->type == SPHERE)
	{
		normal = subtract_points(point, closest_shape->center);
		normal = mult_k_vec(1.0 / length_vec(normal), normal);
	}
	else if (closest_shape->type == PLANE)
	{
		t_point *shape_normal = create_point(0, 1, 0);
		// normal = mult_k_vec(-1.0, shape_normal);
		normal = shape_normal;

	}
	// for STEP 3
	t_point *view = mult_k_vec(-1, direction);

	lighting = compute_lighting(tracer, point, normal, view, closest_shape->specular);

	int local_color = mult_k_color(lighting, closest_shape->color);

	if (closest_shape->reflective <= 0 || depth <= 0)
		return (local_color);

	t_point *reflected_ray = reflect_ray(view, normal);

	int		reflected_color = trace_ray(tracer, point, reflected_ray, EPSILON, INFINIT, depth - 1);

	int tmp_color1 = mult_k_color(1 - closest_shape->reflective, local_color);
	int tmp_color2 = mult_k_color(closest_shape->reflective, reflected_color);

	free(closest_params);
	free(point);
	free(normal);
	free(view);
	free(reflected_ray);
	return(tmp_color1 + tmp_color2);


	// STEP 4
	// return (mult_k_color(lighting, closest_params->closest_sphere->color));

	// for STEP 1
	// return closest_shape->color;
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
		{
			t_point *direction = canvas_to_viewport(x, y);
			rotation_x(tracer);
			direction = mult_vec_matrix(direction, tracer->camera_rotation);
			color = trace_ray(tracer, tracer->camera_position, direction, 1.0, INFINIT, RECURSION_DEPTH);
			put_pixel(tracer, x, y, color);
			free(direction);
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
int main(int argc, char const **argv)
{
	t_tracer	*tracer;

	tracer = (t_tracer*)malloc(sizeof(t_tracer));
	init_struct(tracer);
	tracer->mlx_ptr = mlx_init();
	tracer->win_ptr = mlx_new_window(tracer->mlx_ptr, WIDTH, HEIGHT, "TRACER");
	tracer->img_ptr = mlx_new_image(tracer->mlx_ptr, WIDTH, HEIGHT);
	tracer->image = (int *)mlx_get_data_addr(tracer->img_ptr,
		&tracer->bits_per_pixel, &tracer->size_line, &tracer->endian);


	// CREATE SHAPES
	t_shape *test_sphere = create_shape(SPHERE, create_point(0, -1, 3), 1, 0xFF0000, 500, 0.2);
	add_shape_to_list(&tracer->shapes, test_sphere);

	t_shape *test_plane = create_shape(PLANE, create_point(0, -1, 0), 5, 0x00FF00, 500, 0.2);
	add_shape_to_list(&tracer->shapes, test_plane);


	// CREATE SHERES
	// t_sphere *r_sphere = init_sphere(create_point(0, -1, 3), 1, 0xFF0000, 500, 0.2);
	// add_sphere_to_list(&tracer->spheres, r_sphere);
	// t_sphere *b_sphere = init_sphere(create_point(2, 0, 4), 1, 0x0000FF, 500, 0.3);
	// add_sphere_to_list(&tracer->spheres, b_sphere);
	// t_sphere *g_sphere = init_sphere(create_point(-2, 0, 4), 1, 0x00FF00, 10, 0.4);
	// add_sphere_to_list(&tracer->spheres, g_sphere);
	// t_sphere *y_sphere = init_sphere(create_point(0, -5001, 0), 5000, 0xFFFF00, 1000, 0.5);
	// add_sphere_to_list(&tracer->spheres, y_sphere);

	// print_list_spheres(tracer->spheres);

	// print_list_spheres(tracer->spheres);
	tracer->camera_position = create_point(0, 0, 0);
	init_rotation(tracer);

	// add light to scene
	// t_light *a_light = create_light(create_point(0, 0, 0), AMBIENT, 0.2);
	// add_light_to_list(&tracer->lights, a_light);
	t_light *p_light = create_light(create_point(0, 4, 0), POINT, 0.6);
	add_light_to_list(&tracer->lights, p_light);
	// t_light *d_light = create_light(create_point(1, 4, 4), DIRECTIONAL, 0.2);
	// add_light_to_list(&tracer->lights, d_light);

	// print_list_lights(tracer->lights);

	start_threads(tracer);
	// render(tracer);

	mlx_hook(tracer->win_ptr, 2, 5, choose_key, tracer);
	mlx_loop(tracer->mlx_ptr);

	return (0);
}