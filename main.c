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

void	init_struct(t_tracer *tracer)
{
	tracer->mlx_ptr = NULL;
	tracer->win_ptr = NULL;

	tracer->camera_position = NULL;
	tracer->spheres = NULL;
	tracer->lights = NULL;
}


int		x_exit(void *param)
{
	param = NULL;
	exit(1);
	return (0);
}

int choose_key(int key, void *tracer)
{
	if (key == ESC)
		x_exit(tracer);
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


void	info_about_point(t_point *point)
{
	printf("=======================================\n");
	printf("point->x = %f\n", point->x);
	printf("point->y = %f\n", point->y);
	printf("point->z = %f\n", point->z);
	printf("point->color = %d\n", point->color);
}

void	info_about_sphere(t_sphere *sphere)
{
	printf("=======================================\n");
	printf("sphere->center->x = %f\n", sphere->center->x);
	printf("sphere->center->y = %f\n", sphere->center->y);
	printf("sphere->center->z = %f\n", sphere->center->z);
	printf("shpere->radius = %f\n", sphere->radius);
	printf("sphere->color = %d\n", sphere->color);
	printf("sphere->specular = %f\n", sphere->specular);
	printf("sphere->reflective = %f\n", sphere->reflective);

}

void	info_about_light(t_light *light)
{
	printf("=======================================\n");
	printf("light->position->x = %f\n", light->position->x);
	printf("light->position->y = %f\n", light->position->y);
	printf("light->position->z = %f\n", light->position->z);
	printf("light->type = %d\n", light->type);
	printf("light->intensity = %f\n", light->intensity);
}

void	add_sphere_to_list(t_sphere **head, t_sphere *sphere)
{
	t_sphere *current;

	current = *head;
	if (current == NULL)
	{
		*head = sphere;
	}
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = sphere;
	}
}

void	add_light_to_list(t_light **head, t_light *light)
{
	t_light *current;

	current = *head;
	if (current == NULL)
	{
		*head = light;
	}
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = light;
	}
}

void	print_list_spheres(t_sphere *head)
{
	t_sphere *current;

	current = head;
	while (current != NULL)
	{
		info_about_sphere(current);
		current = current->next;
	}
}

void	print_list_lights(t_light *head)
{
	t_light *current;

	current = head;
	while (current != NULL)
	{
		info_about_light(current);
		current = current->next;
	}
}

void	info_about_matrix4x4(double (*matrix)[4])
{
	int i;
	int j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			printf("%f ", matrix[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

t_sphere *init_sphere(t_point *center, double radius, int color,
										double specular, double reflective)
{
	t_sphere *sphere;

	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	sphere->center = center;
	sphere->radius = radius;
	sphere->color = color;

	sphere->specular = specular;
	sphere->reflective = reflective;
	sphere->next = NULL;
	return (sphere);
}

t_light	*create_light(t_point *position, int type, double intensity)
{
	t_light *new_light;

	new_light = (t_light *)malloc(sizeof(t_light));
	new_light->position = position;
	new_light->type = type;
	new_light->intensity = intensity;

	new_light->next = NULL;
	return(new_light);
}

void convert_to_conterclock(t_point *corners)
{
	for (int i = 0; i < 8; i++)
	{
		corners[i].y *= -1;
	}
}

void draw(t_tracer *tracer)
{
	t_point corners[8] = {
		{ 1, -1, -5, 0xFF0000},
		{ 1, -1, -3, 0x00FF00},
		{ 1,  1, -5, 0x0000FF},
		{ 1,  1, -3, 0xFFFFFF},
		
		{-1, -1, -5, 0xFFFFFF},
		{-1, -1, -3, 0xFFFFFF},
		{-1,  1, -5, 0xFFFFFF},
		{-1,  1, -3, 0xFFFFFF} 
	};


	// convert_to_conterclock(corners);
	int color = 0xFF0000;

	// get object's points on the canvas
	for (int i = 0; i < 8; i++)
	{
		// divide the x and y coordinates by the z coordinate to 
		// project the point on the canvas
		double x_proj = corners[i].x / -corners[i].z;
		double y_proj = corners[i].y / -corners[i].z;
		printf("projected corner: %d x:%f y:%f\n", i, x_proj, y_proj);

		// normalize points to range [0, 1]
		double x_proj_remap = (1 + x_proj) / 2;
		double y_proj_remap = (1 + y_proj) / 2;
		printf("remap corner: %d x:%f y:%f\n", i, x_proj_remap, y_proj_remap);


		double x_proj_pix = x_proj_remap * WIDTH;
		double y_proj_pix = y_proj_remap * HEIGHT;
		printf("in pixels: %d x:%f y:%f\n", i, x_proj_pix, y_proj_pix);

		mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, 
			x_proj_pix, y_proj_pix, corners[i].color);		

		printf("\n");
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

	mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, screen_x, screen_y, color);
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

double	*intersect_ray_sphere(t_tracer *tracer, t_point *origin, t_point *direction, t_sphere *sphere)
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

	oc = subtract_points(origin, sphere->center);

	k1 = dot_product(direction, direction);
	k2 = 2.0 * dot_product(oc, direction);
	k3 = dot_product(oc, oc) - sphere->radius * sphere->radius;

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

t_closest	*init_closest(void)
{
	t_closest	*closest;

	closest = (t_closest *)malloc(sizeof(t_closest));
	closest->closest_t = INFINIT;
	closest->closest_sphere = NULL;
	return (closest);
}

t_closest	*closest_intersection(t_tracer *tracer, t_point *origin, t_point *direction,
											double min_t, double max_t)
{
	t_sphere	*current_sphere;
	double		*ts;
	t_closest	*closest_params;

	closest_params = init_closest();
	current_sphere = tracer->spheres;
	while (current_sphere != NULL)
	{
		ts = intersect_ray_sphere(tracer, origin, direction, current_sphere);
		if (ts[0] < closest_params->closest_t && min_t < ts[0] && ts[0] < max_t)
		{
			closest_params->closest_t = ts[0];
			closest_params->closest_sphere = current_sphere;
		}
		if (ts[1] < closest_params->closest_t && min_t < ts[1] && ts[1] < max_t)
		{
			closest_params->closest_t = ts[1];
			closest_params->closest_sphere = current_sphere;	
		}
		current_sphere = current_sphere->next;
	}
	if (closest_params->closest_sphere != NULL)
		return (closest_params);
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

	t_sphere *closest_sphere = closest_params->closest_sphere;
	double	closest_t = closest_params->closest_t;

	// added in STEP 2
	t_point		*point;
	t_point		*normal;
	double		lighting;

	point = add_points(origin, mult_k_vec(closest_t, direction));
	normal = subtract_points(point, closest_sphere->center);
	normal = mult_k_vec(1.0 / length_vec(normal), normal);
	
	// for STEP 3
	t_point *view = mult_k_vec(-1, direction);

	lighting = compute_lighting(tracer, point, normal, view, closest_sphere->specular);

	int local_color = mult_k_color(lighting, closest_sphere->color);


	if (closest_sphere->reflective <= 0 || depth <= 0)
		return (local_color);

	t_point *reflected_ray = reflect_ray(view, normal);

	int		reflected_color = trace_ray(tracer, point, reflected_ray, EPSILON, INFINIT, depth - 1);

	int tmp_color1 = mult_k_color(1 - closest_sphere->reflective, local_color);
	int tmp_color2 = mult_k_color(closest_sphere->reflective, reflected_color);

	return(tmp_color1 + tmp_color2);


	// STEP 4
	// return (mult_k_color(lighting, closest_params->closest_sphere->color));

	// for STEP 1
	// return closest_sphere->color;
}

void	render(t_tracer *tracer)
{
	int x;
	int y;
	int color;

	x = -WIDTH / 2;
	while (x < WIDTH / 2)
	{
		y = -HEIGHT / 2;
		while (y < HEIGHT / 2)
		{
			t_point *direction = canvas_to_viewport(x, y);
			
			color = trace_ray(tracer, tracer->camera_position, direction, 1.0, INFINIT, RECURSION_DEPTH);
			put_pixel(tracer, x, y, color);
			y++;
		}
		x++;
	}
}


// compilation without FLAGS
int main(int argc, char const **argv)
{
	t_tracer	*tracer;

	tracer = (t_tracer*)malloc(sizeof(t_tracer));
	init_struct(tracer);
	tracer->mlx_ptr = mlx_init();
	tracer->win_ptr = mlx_new_window(tracer->mlx_ptr, WIDTH, HEIGHT, "TRACER");

	// draw(tracer);
	// t_sphere *spheres = NULL;

	t_sphere *r_sphere = init_sphere(create_point(0, -1, 3), 1, 0xFF0000, 500, 0.2);
	add_sphere_to_list(&tracer->spheres, r_sphere);
	t_sphere *b_sphere = init_sphere(create_point(2, 0, 4), 1, 0x0000FF, 500, 0.3);
	add_sphere_to_list(&tracer->spheres, b_sphere);
	t_sphere *g_sphere = init_sphere(create_point(-2, 0, 4), 1, 0x00FF00, 10, 0.4);
	add_sphere_to_list(&tracer->spheres, g_sphere);
	t_sphere *y_sphere = init_sphere(create_point(0, -5001, 0), 5000, 0xFFFF00, 1000, 0.5);
	add_sphere_to_list(&tracer->spheres, y_sphere);

	// print_list_spheres(tracer->spheres);

	// print_list_spheres(tracer->spheres);
	tracer->camera_position = create_point(0, 0, 0);

	// add light to scene
	// t_light *lights = NULL;

	t_light *a_light = create_light(create_point(0, 0, 0), AMBIENT, 0.2);
	add_light_to_list(&tracer->lights, a_light);
	t_light *p_light = create_light(create_point(2, 1, 0), POINT, 0.6);
	add_light_to_list(&tracer->lights, p_light);
	t_light *d_light = create_light(create_point(1, 4, 4), DIRECTIONAL, 0.2);
	add_light_to_list(&tracer->lights, d_light);

	// print_list_lights(tracer->lights);

	render(tracer);

	mlx_hook(tracer->win_ptr, 2, 5, choose_key, tracer);
	mlx_loop(tracer->mlx_ptr);

	return (0);
}