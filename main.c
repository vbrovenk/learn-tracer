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

void	draw_line(t_tracer *tracer, t_point p1, t_point p2)
{
	int dx = (p2.x - p1.x >= 0 ? 1 : -1);
	int dy = (p2.y - p1.y >= 0 ? 1 : -1);

	double lengthX = abs(p2.x - p1.x);
	double lengthY = abs(p2.y - p1.y);

	double length = fmax(lengthX, lengthY);

	if (length == 0)
		mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, p1.x, p1.y, 0x00FF00);
	if (lengthY <= lengthX)
	{
		double x = p1.x;
		double y = p1.y;
		double d = -lengthX;

		length++;
		while (length--)
		{
			mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, x, y, 0x00FF00);
			x += dx;
			d += 2 * lengthY;
			if (d > 0)
			{
				d -= 2 * lengthX;
				y += dy;
			}
		}
	}
	else
	{
		double x = p1.x;
		double y = p1.y;
		double d = -lengthY;

		length++;
		while (length--)
		{
			mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, x, y, 0x00FF00);
			y += dy;
			d += 2 * lengthX;
			if (d > 0)
			{
				d -= 2 * lengthY;
				x += dx;
			}
		}

	}
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

void	print_list(t_sphere *head)
{
	t_sphere *current;

	current = head;
	while (current != NULL)
	{
		info_about_sphere(current);
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

t_sphere *init_sphere(t_point *center, double radius, int color)
{
	t_sphere *sphere;

	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	sphere->center = center;
	sphere->radius = radius;
	sphere->color = color;

	sphere->next = NULL;
	return (sphere);
}

t_sphere *create_spheres()
{
	t_sphere *spheres;



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

	double	*res = (double *)malloc(sizeof(double) * 2);

	oc = subtraction_points(tracer->camera_position, sphere->center);

	k1 = dot_product(direction, direction);
	k2 = 2 * dot_product(oc, direction);
	k3 = dot_product(oc, oc) - sphere->radius * sphere->radius;

	discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0)
	{
		res[0] = INFINIT;
		res[1] = INFINIT;
		return (res);
	}

	t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
	t2 = (-k2 - sqrt(discriminant)) / (2 * k1);
	res[0] = t1;
	res[1] = t2;
	return (res);
}

int		trace_ray(t_tracer *tracer, t_point *direction, t_sphere *spheres)
{
	int closest_t = INFINIT;
	t_sphere *closest_sphere = NULL;
	t_sphere *current_sphere;
	double *ts;

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
		return (0xFFFFFF);

	return closest_sphere->color;
}

void	render(t_tracer *tracer, t_sphere *spheres)
{
	int x;
	int y;

	x = -WIDTH / 2;
	while (x < WIDTH / 2)
	{
		y = -HEIGHT / 2;
		while (y < HEIGHT / 2)
		{
			t_point *direction = canvas_to_viewport(x, y);
			int color = trace_ray(tracer, direction, spheres);
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

	t_sphere *r_sphere = init_sphere(create_point(0, -1, 3), 1, 0xFF0000);
	add_sphere_to_list(&spheres, r_sphere);
	t_sphere *b_sphere = init_sphere(create_point(2, 0, 4), 1, 0x0000FF);
	add_sphere_to_list(&spheres, b_sphere);
	t_sphere *g_sphere = init_sphere(create_point(-2, 0, 4), 1, 0x00FF00);
	add_sphere_to_list(&spheres, g_sphere);

	tracer->camera_position = create_point(0, 0, 0);	
	render(tracer, spheres);

	mlx_hook(tracer->win_ptr, 2, 5, choose_key, tracer);
	mlx_loop(tracer->mlx_ptr);

	return (0);
}