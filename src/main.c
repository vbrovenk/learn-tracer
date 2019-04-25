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

int		normal_light(t_tracer *tracer, t_closest *closest_params,
												t_point *point, t_point *origin)
{
	t_point	*normal;
	double	lighting;
	int		local_color;

	normal = NULL;
	if (closest_params->closest_shape->type == SPHERE)
		normal = sphere_normal(closest_params, point);
	else if (closest_params->closest_shape->type == PLANE)
		normal = plane_normal(closest_params);
	else if (closest_params->closest_shape->type == CYLINDER)
		normal = cylinder_normal(closest_params, point, origin, tracer->d);
	else if (closest_params->closest_shape->type == CONE)
		normal = cone_normal(closest_params, point, origin, tracer->d);
	lighting = compute_lighting(tracer, point, normal,
							closest_params->closest_shape->specular);
	local_color = mult_k_color(lighting, closest_params->closest_shape->color);
	free(normal);
	return (local_color);
}

int		trace_ray(t_tracer *tracer, t_point *origin, double t_min, double t_max)
{
	t_closest	*closest_params;
	t_point		*point;
	int			local_color;
	double		min_max[2];
	t_point		*temp;

	min_max[0] = t_min;
	min_max[1] = t_max;
	closest_params = closest_intersection(tracer, origin, tracer->d, min_max);
	if (closest_params == NULL)
		return (BACKGROUND);
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
		{
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
	int			i;
	int			temp_start;

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
	mlx_put_image_to_window(tracer->mlx_ptr, tracer->win_ptr,
												tracer->img_ptr, 0, 0);
}

// compilation without FLAGS
int	main(int argc, char *argv[])
{
	t_tracer	*tracer;
	char		*extension;

	if (argc != 2)
		print_error("Usage: ./RTv1 scene.txt");
	if (ft_strlen(argv[1]) < 5 || ft_strrchr(argv[1], '.') == NULL)
		print_error("Wrong file");
	extension = ft_strrchr(argv[1], '.');
	if (ft_strequ(extension, ".txt") == 0)
		print_error("Wrong extension of file");
	tracer = init_struct();
	tracer->mlx_ptr = mlx_init();
	tracer->win_ptr = mlx_new_window(tracer->mlx_ptr, WIDTH, HEIGHT, "TRACER");
	tracer->img_ptr = mlx_new_image(tracer->mlx_ptr, WIDTH, HEIGHT);
	tracer->image = (int *)mlx_get_data_addr(tracer->img_ptr,
		&tracer->bits_per_pixel, &tracer->size_line, &tracer->endian);
	read_data(tracer, argv[1]);
	if (tracer->camera_position == NULL)
		print_error("Camera must be set");
	init_rotation(tracer);
	start_threads(tracer);
	mlx_hook(tracer->win_ptr, 2, 5, choose_key, tracer);
	mlx_hook(tracer->win_ptr, 17, 1L << 17, x_exit, 0);
	mlx_loop(tracer->mlx_ptr);
	return (0);
}
