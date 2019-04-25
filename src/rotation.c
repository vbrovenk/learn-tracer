/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 17:30:21 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 17:30:23 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

void	init_rotation(t_tracer *tracer)
{
	int i;
	int j;

	tracer->camera_rotation = (t_m3x3 *)malloc(sizeof(t_m3x3));
	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
			tracer->camera_rotation->matrix[i][j] = 0;
	}
	tracer->camera_rotation->matrix[0][0] = 1;
	tracer->camera_rotation->matrix[1][1] = 1;
	tracer->camera_rotation->matrix[2][2] = 1;
}

void	rotation_x(t_tracer *tracer)
{
	tracer->camera_rotation->matrix[1][1] = cos(tracer->degrees_x * M_PI / 180);
	tracer->camera_rotation->matrix[1][2] = -sin(tracer->degrees_x * M_PI / 180);
	tracer->camera_rotation->matrix[2][1] = sin(tracer->degrees_x * M_PI / 180);
	tracer->camera_rotation->matrix[2][2] = cos(tracer->degrees_x * M_PI / 180);
}

void	rotation_y(t_tracer *tracer)
{
	tracer->camera_rotation->matrix[0][0] = cos(tracer->degrees_y * M_PI / 180);
	tracer->camera_rotation->matrix[0][2] = sin(tracer->degrees_y * M_PI / 180);
	tracer->camera_rotation->matrix[2][0] = -sin(tracer->degrees_y * M_PI / 180);
	tracer->camera_rotation->matrix[2][2] = cos(tracer->degrees_y * M_PI / 180);
}
