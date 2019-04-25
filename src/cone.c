/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 11:34:33 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 11:34:34 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

double	cut_cone(t_point *origin, t_shape *shape, double value, double res)
{
	t_point	*temp;
	double	coeff;

	temp = subtract_points(origin, shape->center);
	coeff = value * res + dot_product(temp, shape->dir);
	free(temp);
	if (coeff < -shape->height_cone2 || coeff > shape->height_cone1)
		return (INFINIT);
	return (res);
}

double	*find_intersect(double *res, double discriminant, double *coeff)
{
	if (discriminant < 0)
	{
		res[0] = INFINIT;
		res[1] = INFINIT;
		return (res);
	}
	res[0] = (-coeff[1] + discriminant) / (2.0 * coeff[0]);
	res[1] = (-coeff[1] - discriminant) / (2.0 * coeff[0]);
	return (res);
}

double	*intersect_ray_cone(t_point *origin, t_point *direction, t_shape *shape)
{
	double	coeff[3];
	double	discriminant;
	t_point	*oc;
	double	value[2];
	double	*res;

	res = malloc(sizeof(double) * 2);
	value[0] = dot_product(direction, shape->dir);
	value[1] = 1 + shape->angle * shape->angle;
	oc = subtract_points(origin, shape->center);
	coeff[0] = dot_product(direction, direction) -
										value[1] * value[0] * value[0];
	coeff[1] = 2 * (dot_product(direction, oc) - value[1] * value[0] *
											dot_product(oc, shape->dir));
	coeff[2] = dot_product(oc, oc) - value[1] *
									pow(dot_product(oc, shape->dir), 2.0);
	free(oc);
	discriminant = sqrt(coeff[1] * coeff[1] - 4 * coeff[0] * coeff[2]);
	res = find_intersect(res, discriminant, coeff);
	if (discriminant < 0)
		return (res);
	res[0] = cut_cone(origin, shape, value[0], res[0]);
	res[1] = cut_cone(origin, shape, value[0], res[1]);
	return (res);
}

t_point	*cone_normal(t_closest *closest_params, t_point *point,
						t_point *origin, t_point *direction)
{
	double	coeff;
	t_point	*temp;
	t_point	*temp2;
	t_point	*temp3;
	t_point	*normal;

	temp = subtract_points(origin, closest_params->closest_shape->center);
	coeff = dot_product(direction, closest_params->closest_shape->dir) *
									closest_params->closest_v +
					dot_product(temp, closest_params->closest_shape->dir);
	free(temp);
	temp = subtract_points(point, closest_params->closest_shape->center);
	temp3 = mult_k_vec(coeff, closest_params->closest_shape->dir);
	temp2 = mult_k_vec(1 + pow(closest_params->closest_shape->angle, 2.0),
																	temp3);
	normal = subtract_points(temp, temp2);
	free(temp);
	free(temp2);
	free(temp3);
	return (normal);
}
