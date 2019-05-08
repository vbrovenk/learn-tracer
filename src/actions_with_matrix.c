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

t_point		*normalize(t_point *p)
{
	double	n;
	double	factor;
	t_point	*new;

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

double		length_vec(t_point *vec)
{
	double length;

	length = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	return (length);
}
