/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 17:01:33 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 17:01:35 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

t_closest	*init_closest(void)
{
	t_closest	*closest;

	closest = (t_closest *)malloc(sizeof(t_closest));
	closest->closest_v = INFINIT;
	closest->closest_shape = NULL;
	return (closest);
}

double		*determine_shape(t_point *origin, t_point *d,
											t_shape *current_shape, double *ts)
{
	if (current_shape->type == SPHERE)
		ts = intersect_ray_sphere(origin, d, current_shape);
	else if (current_shape->type == PLANE)
		ts = intersect_ray_plane(origin, d, current_shape);
	else if (current_shape->type == CYLINDER)
		ts = intersect_ray_cylinder(origin, d, current_shape);
	else if (current_shape->type == CONE)
		ts = intersect_ray_cone(origin, d, current_shape);
	return (ts);
}

void		determine_nearest(t_shape *current_shape, double *ts,
									t_closest *closest_params, double *min_max)
{
	if (ts[0] < closest_params->closest_v &&
						min_max[0] < ts[0] && ts[0] < min_max[1])
	{
		closest_params->closest_v = ts[0];
		closest_params->closest_shape = current_shape;
	}
	if (ts[1] < closest_params->closest_v &&
						min_max[0] < ts[1] && ts[1] < min_max[1])
	{
		closest_params->closest_v = ts[1];
		closest_params->closest_shape = current_shape;
	}
}

t_closest	*closest_intersection(t_tracer *tracer, t_point *origin, t_point *d,
																double *min_max)
{
	t_shape		*current_shape;
	double		*ts;
	t_closest	*closest_params;

	ts = NULL;
	closest_params = init_closest();
	current_shape = tracer->shapes;
	while (current_shape != NULL)
	{
		if (ts != NULL)
			free(ts);
		ts = determine_shape(origin, d, current_shape, ts);
		determine_nearest(current_shape, ts, closest_params, min_max);
		current_shape = current_shape->next;
	}
	if (ts != NULL)
		free(ts);
	if (closest_params->closest_shape != NULL)
		return (closest_params);
	free(closest_params);
	return (NULL);
}
