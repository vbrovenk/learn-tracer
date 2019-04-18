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