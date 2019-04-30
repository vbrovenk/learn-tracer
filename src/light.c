/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 14:51:54 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 14:51:56 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

t_point		*reflect_ray(t_point *vec1, t_point *vec2)
{
	t_point *temp;
	t_point *sub;

	temp = mult_k_vec(2 * dot_product(vec1, vec2), vec2);
	sub = subtract_points(temp, vec1);
	free(temp);
	return (sub);
}

void		compute_specular(t_point *normal, double specular, t_specular *spec, t_point *view)
{
	double		n_dot_l;
	t_point		*vec_r;
	double		r_dot_v;

	n_dot_l = dot_product(normal, spec->vec_l);
	if (n_dot_l > 0)
		spec->intensity += spec->current_light->intensity * n_dot_l /
								(length_vec(normal) * length_vec(spec->vec_l));
	if (specular >= 0)
	{
		vec_r = reflect_ray(spec->vec_l, normal);
		r_dot_v = dot_product(vec_r, view);
		if (r_dot_v > 0)
			spec->intensity += spec->current_light->intensity * pow(r_dot_v /
						(length_vec(vec_r) * length_vec(view)), specular);
		free(vec_r);
	}
}

t_specular	*init_specular(t_tracer *tracer)
{
	t_specular	*spec;

	if ((spec = (t_specular *)malloc(sizeof(t_specular))) == NULL)
		print_error("Can't malloc memory for t_specular");

	spec->intensity = 0;
	spec->current_light = tracer->lights;
	return (spec);
}

double		compute_lighting(t_tracer *tracer, t_point *point, t_point *normal,
											double specular, t_point *view)
{
	double		min_max[2];
	t_closest	*blocker;
	t_specular	*spec;

	spec = init_specular(tracer);
	while (spec->current_light != NULL)
	{
		spec->vec_l = subtract_points(spec->current_light->position, point);
		min_max[0] = EPSILON;
		min_max[1] = 1.0;
		blocker = closest_intersection(tracer, point, spec->vec_l, min_max);
		if (blocker != NULL)
		{
			free(blocker);
			free(spec->vec_l);
			spec->current_light = spec->current_light->next;
			continue ;
		}
		compute_specular(normal, specular, spec, view);
		free(spec->vec_l);
		spec->current_light = spec->current_light->next;
	}
	free(spec);
	return (spec->intensity);
}
