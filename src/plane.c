/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 11:14:27 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 11:14:29 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

double	*intersect_ray_plane(t_tracer *tracer, t_point *origin, t_point *direction, t_shape *shape)
{
	double	*res;
	t_point	*x;

	res = malloc(sizeof(double) * 2);
	x = subtract_points(origin, shape->center);
	res[0] = dot_product(direction, shape->dir);
	res[1] = dot_product(x, shape->dir);
	if (res[0])
	{
		res[0] = -res[1] / res[0];
		res[1] = INFINIT;
		free(x);
		return (res);
	}
	free(x);
	res[0] = INFINIT;
	res[1] = INFINIT;
	return (res);
}

t_point	*plane_normal(t_closest *closest_params)
{
	t_point	*shape_normal;

	shape_normal = create_point(closest_params->closest_shape->dir->x,
								closest_params->closest_shape->dir->y,
								closest_params->closest_shape->dir->z);
	return (shape_normal);
}
