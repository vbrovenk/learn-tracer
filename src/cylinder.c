/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 11:17:32 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 11:17:34 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

double	*cut_cylinder(t_point *origin, t_shape *shape, double value,
																double *res)
{
	t_point	*temp;
	double	coeff;

	if (shape->height_cylinder != INFINIT)
	{
		temp = subtract_points(origin, shape->center);
		coeff = value * res[0] + dot_product(temp, shape->dir);
		free(temp);
		if (coeff < 0.0 || coeff > shape->height_cylinder)
			res[0] = INFINIT;
		temp = subtract_points(origin, shape->center);
		coeff = value * res[1] + dot_product(temp, shape->dir);
		free(temp);
		if (coeff < 0.0 || coeff > shape->height_cylinder)
			res[1] = INFINIT;
	}
	return (res);
}

double	*intersect_ray_cylinder(t_tracer *tracer, t_point *origin,
								t_point *direction, t_shape *shape)
{
	double	coeff[3];
	double	discriminant;
	t_point	*oc;
	double	value;
	double	*res;

	res = malloc(sizeof(double) * 2);
	value = dot_product(direction, shape->dir);
	oc = subtract_points(origin, shape->center);
	coeff[0] = dot_product(direction, direction) - value * value;
	coeff[1] = 2.0 * (dot_product(direction, oc) - value *
											dot_product(oc, shape->dir));
	coeff[2] = dot_product(oc, oc) - pow(dot_product(oc, shape->dir), 2.0)
												- pow(shape->radius, 2.0);
	free(oc);
	discriminant = sqrt(coeff[1] * coeff[1] - 4.0 * coeff[0] * coeff[2]);
	if (discriminant < 0)
	{
		res[0] = INFINIT;
		res[1] = INFINIT;
		return (res);
	}
	res[0] = (-coeff[1] + discriminant) / (2.0 * coeff[0]);
	res[1] = (-coeff[1] - discriminant) / (2.0 * coeff[0]);
	return (cut_cylinder(origin, shape, value, res));
}

t_point	*cylinder_normal(t_closest *closest_params, t_point *point,
						t_point *origin, t_point *direction)
{
	double	coeff;
	t_point	*temp;
	t_point	*temp2;
	t_point	*normal;

	temp = subtract_points(origin, closest_params->closest_shape->center);
	coeff = dot_product(direction, closest_params->closest_shape->dir) *
									closest_params->closest_v +
				dot_product(temp, closest_params->closest_shape->dir);
	free(temp);
	temp = subtract_points(point, closest_params->closest_shape->center);
	temp2 = mult_k_vec(coeff, closest_params->closest_shape->dir);
	normal = subtract_points(temp, temp2);
	free(temp);
	free(temp2);
	return (normal);
}
