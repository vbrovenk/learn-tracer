/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 17:43:59 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 17:44:00 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

t_tracer	*init_struct(void)
{
	t_tracer *tracer;

	tracer = (t_tracer*)malloc(sizeof(t_tracer));
	tracer->mlx_ptr = NULL;
	tracer->win_ptr = NULL;
	tracer->camera_position = NULL;
	tracer->shapes = NULL;
	tracer->lights = NULL;
	tracer->d = NULL;
	tracer->degrees_x = 0;
	tracer->degrees_y = 0;
	return (tracer);
}

int			x_exit(void *param)
{
	param = NULL;
	exit(1);
	return (0);
}

void		new_position(t_tracer *tracer, int degrees_x, int degrees_y,
															double coeff)
{
	t_point *new_camera;
	t_point *temp;
	t_point *temp2;

	new_camera = canvas_to_viewport(0, 0);
	temp = new_camera;
	new_camera = rotation_x(new_camera, degrees_x);
	free(temp);
	temp = new_camera;
	new_camera = rotation_y(new_camera, degrees_y);
	free(temp);
	temp = tracer->camera_position;
	temp2 = mult_k_vec(coeff, new_camera);
	tracer->camera_position = add_points(tracer->camera_position, temp2);
	free(temp2);
	free(temp);
	free(new_camera);
}

int			choose_key(int key, t_tracer *tracer)
{
	if (key == ESC)
		x_exit(tracer);
	else if (key == ARROW_UP)
		new_position(tracer, tracer->degrees_x, tracer->degrees_y, 1);
	else if (key == ARROW_DOWN)
		new_position(tracer, tracer->degrees_x, tracer->degrees_y, -1);
	else if (key == ARROW_LEFT)
		new_position(tracer, 0, tracer->degrees_y - 90, 1);
	else if (key == ARROW_RIGHT)
		new_position(tracer, 0, tracer->degrees_y + 90, 1);
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
	start_threads(tracer);
	return (0);
}

void		print_error(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(1);
}
