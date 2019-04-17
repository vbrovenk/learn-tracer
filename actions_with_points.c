/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_with_points.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 16:57:51 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/08/21 16:57:52 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

t_point *create_point(double x, double y, double z)
{
	t_point *new;

	if ((new = (t_point *)malloc(sizeof(t_point))) == NULL)
		print_error("Can't malloc memory for t_point");
	new->x = x;
	new->y = y;
	new->z = z;
	new->color = 0xFFFFFF;
	return (new);
}

t_point *subtract_points(t_point *end, t_point *start)
{
	t_point *new;

	new = (t_point *)malloc(sizeof(t_point));
	new->x = end->x - start->x;
	new->y = end->y - start->y;
	new->z = end->z - start->z;
	return (new);
}

t_point *add_points(t_point *end, t_point *start)
{
	t_point *new;

	new = (t_point *)malloc(sizeof(t_point));
	new->x = end->x + start->x;
	new->y = end->y + start->y;
	new->z = end->z + start->z;
	return (new);
}

double	dot_product(t_point *p1, t_point *p2)
{
	double result;

	result = p1->x * p2->x + p1->y * p2->y + p1->z * p2->z;
	return (result);
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

t_point *mult_k_vec(double k, t_point *vec)
{
	double x;
	double y;
	double z;

	x = k * vec->x;
	y = k * vec->y;
	z = k * vec->z;
	return (create_point(x, y, z));
}

double	length_vec(t_point *vec)
{
	double length;

	length = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	return (length);
}
