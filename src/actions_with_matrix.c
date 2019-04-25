/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_with_matrix.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 16:57:51 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/08/21 16:57:52 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

t_point *mult_vec_matrix(t_point *vec, t_m3x3 *m) // vec will free here
{
	double a;
	double b;
	double c;

	a = vec->x * m->matrix[0][0] + vec->y * m->matrix[0][1] + vec->z * m->matrix[0][2];
	b = vec->x * m->matrix[1][0] + vec->y * m->matrix[1][1] + vec->z * m->matrix[1][2];
	c = vec->x * m->matrix[2][0] + vec->y * m->matrix[2][1] + vec->z * m->matrix[2][2];
	free(vec);
	return (create_point(a, b, c));
}

t_point *normalize(t_point *p)
{
	double n;
	double factor;
	t_point *new;

	new = create_point(0, 0, 0);
	n = p->x * p->x + p->y * p->y + p->z * p->z;
	if (n > 0)
	{
		factor = 1 / sqrt(n);
		new->x = p->x * factor;
		new->y = p->y * factor;
		new->z = p->z * factor;
	}
	else
	{
		new->x = p->x;
		new->y = p->y;
		new->z = p->z;
	}
	return (new);
}

double	length_vec(t_point *vec)
{
	double length;

	length = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	return (length);
}
