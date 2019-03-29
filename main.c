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

void	init_struct(t_tracer *fdf)
{
	fdf->mlx_ptr = NULL;
	fdf->win_ptr = NULL;

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

t_sphere *init_sphere(t_point *center, double radius, int color, double specular)
{
	t_sphere *sphere;

	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	sphere->center = center;
	sphere->radius = radius;
	sphere->color = color;

	sphere->specular = specular;
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

double	*intersect_ray_sphere(t_tracer *tracer, t_point *direction, t_sphere *sphere)
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

	oc = subtract_points(tracer->camera_position, sphere->center);

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

double	compute_lighting(t_light *lights, t_point *point, t_point *normal,
													t_point *view, double specular)
{
	double		intensity;
	double		length_n;
	t_light		*current;
	double		n_dot_l;
	t_point		*vec_l;
	// for specular
	double		length_v = length_vec(view);

	length_n = length_vec(normal);
	current = lights;
	intensity = 0;
	while (current != NULL)
	{
		if (current->type == AMBIENT)
			intensity += current->intensity;
		else
		{
			if (current->type == POINT)
				vec_l = subtract_points(current->position, point);
			else // DIRECTIONAL
				vec_l = current->position;

			// Diffuse reflection
			n_dot_l = dot_product(normal, vec_l);
			if (n_dot_l > 0)
				intensity += current->intensity * n_dot_l / (length_n * length_vec(vec_l));

			// Specular reflection
			if (specular != -1)
			{
				t_point *vec_r;
				double	r_dot_l;
				t_point *mult_vec;

				mult_vec = mult_k_vec(2.0 * dot_product(normal, vec_l), normal);
				vec_r = subtract_points(mult_vec, vec_l);
				r_dot_l = dot_product(vec_r, view);
				if (r_dot_l > 0)
					intensity += current->intensity * pow(r_dot_l / (length_vec(vec_r) * length_v), specular);
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

int		trace_ray(t_tracer *tracer, t_point *direction, t_sphere *spheres, t_light *lights)
{
	double		closest_t;
	t_sphere	*closest_sphere;
	t_sphere	*current_sphere;
	double		*ts;

	closest_t = INFINIT;
	closest_sphere = NULL;
	current_sphere = spheres;
	while (current_sphere != NULL)
	{
		ts = intersect_ray_sphere(tracer, direction, current_sphere);
		if (ts[0] < closest_t && MIN_T < ts[0] && ts[0] < INFINIT)
		{
			closest_t = ts[0];
			closest_sphere = current_sphere;
		}
		if (ts[1] < closest_t && MIN_T < ts[1] && ts[1] < INFINIT)
		{
			closest_t = ts[1];
			closest_sphere = current_sphere;	
		}
		current_sphere = current_sphere->next;
	}

	if (closest_sphere == NULL)
		return (BACKGROUND);

	// added in STEP 2
	t_point		*point;
	t_point		*normal;
	double		lighting;

	point = add_points(tracer->camera_position, mult_k_vec(closest_t, direction));
	normal = subtract_points(point, closest_sphere->center);
	normal = mult_k_vec(1.0 / length_vec(normal), normal);
	
	// for STEP 3
	t_point *view = mult_k_vec(-1, direction);

	lighting = compute_lighting(lights, point, normal, view, closest_sphere->specular);
	return (mult_k_color(lighting, closest_sphere->color));

	// for STEP 1
	// return closest_sphere->color;
}

void	render(t_tracer *tracer, t_sphere *spheres, t_light *lights)
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
			color = trace_ray(tracer, direction, spheres, lights);
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
	t_sphere *spheres = NULL;

	t_sphere *r_sphere = init_sphere(create_point(0, -1, 3), 1, 0xFF0000, 500);
	add_sphere_to_list(&spheres, r_sphere);
	t_sphere *b_sphere = init_sphere(create_point(2, 0, 4), 1, 0x0000FF, 500);
	add_sphere_to_list(&spheres, b_sphere);
	t_sphere *g_sphere = init_sphere(create_point(-2, 0, 4), 1, 0x00FF00, 10);
	add_sphere_to_list(&spheres, g_sphere);
	t_sphere *y_sphere = init_sphere(create_point(0, -5001, 0), 5000, 0xFFFF00, 1000);
	add_sphere_to_list(&spheres, y_sphere);

	tracer->camera_position = create_point(0, 0, 0);

	// add light to scene
	t_light *lights = NULL;

	t_light *a_light = create_light(create_point(0, 0, 0), AMBIENT, 0.2);
	add_light_to_list(&lights, a_light);
	t_light *p_light = create_light(create_point(2, 1, 0), POINT, 0.6);
	add_light_to_list(&lights, p_light);
	t_light *d_light = create_light(create_point(1, 4, 4), DIRECTIONAL, 0.2);
	add_light_to_list(&lights, d_light);

	// print_list_lights(lights);

	render(tracer, spheres, lights);

	mlx_hook(tracer->win_ptr, 2, 5, choose_key, tracer);
	mlx_loop(tracer->mlx_ptr);

	return (0);
}