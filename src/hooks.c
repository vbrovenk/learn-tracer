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
	tracer->camera_rotation = NULL;
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

int			choose_key(int key, t_tracer *tracer)
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
	start_threads(tracer);
	return (0);
}

void		print_error(char *msg)
{
	ft_putendl_fd(msg, 2);
	exit(1);
}
