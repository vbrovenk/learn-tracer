/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 11:06:57 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 11:06:58 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

double	*intersect_ray_sphere(t_tracer *tracer, t_point *origin, t_point *direction, t_shape *shape)
{
	t_point	*oc;
	double	coeff[3];
	double	discriminant;
	double	t[2];
	double	*res;

	res = malloc(sizeof(double) * 2);
	oc = subtract_points(origin, shape->center);
	coeff[0] = dot_product(direction, direction);
	coeff[1] = 2.0 * dot_product(oc, direction);
	coeff[2] = dot_product(oc, oc) - shape->radius * shape->radius;
	discriminant = coeff[1] * coeff[1] - 4.0 * coeff[0] * coeff[2];
	if (discriminant < 0.0)
	{
		res[0] = INFINIT;
		res[1] = INFINIT;
		free(oc);
		return (res);
	}
	t[0] = (-coeff[1] - sqrt(discriminant)) / (double)(2.0 * coeff[0]);
	t[1] = (-coeff[1] + sqrt(discriminant)) / (double)(2.0 * coeff[0]);
	res[0] = t[0];
	res[1] = t[1];
	free(oc);
	return (res);
}

t_point *sphere_normal(t_closest *closest_params, t_point *point)
{
	t_point	*temp;
	t_point	*normal;

	temp = subtract_points(point, closest_params->closest_shape->center);
	normal = mult_k_vec(1.0 / length_vec(temp), temp);
	free(temp);
	return (normal);
}
