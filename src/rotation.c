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


// t_vec		rot_y(t_vec v, int angle)
// {
// 	double	x;
// 	double	z;

// 	x = v.x;
// 	z = v.z;
// 	v.x = x * cos(RAD(angle)) + z * sin(RAD(angle));
// 	v.z = -x * sin(RAD(angle)) + z * cos(RAD(angle));
// 	return (v);
// }


t_point	*rotation_x(t_point *direction, int angle)
{
	t_point *new_direction;

	new_direction = create_point(direction->x, direction->y, direction->z);
	new_direction->y = direction->y *
		cos(angle * M_PI / 180) - direction->z * sin(angle * M_PI / 180);
	new_direction->z = direction->y * sin(angle * M_PI / 180) +
									direction->z * cos(angle * M_PI / 180);
	return (new_direction);
}

t_point	*rotation_y(t_point *direction, int angle)
{
	t_point *new_direction;

	new_direction = create_point(direction->x, direction->y, direction->z);
	new_direction->x = direction->x *
		cos(angle * M_PI / 180) + direction->z * sin(angle * M_PI / 180);
	new_direction->z = -direction->x * sin(angle * M_PI / 180) +
									direction->z * cos(angle * M_PI / 180);
	return (new_direction);
}
