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

t_point *mult_vec_matrix(t_point *vec, double (*m)[4])
{
	double a;
	double b;
	double c;
	double w;

	a = vec->x * m[0][0] + vec->y * m[1][0] + vec->z * m[2][0] + m[3][0];
	b = vec->x * m[0][1] + vec->y * m[1][1] + vec->z * m[2][1] + m[3][1];
	c = vec->x * m[0][2] + vec->y * m[1][2] + vec->z * m[2][2] + m[3][2];
	w = vec->x * m[0][3] + vec->y * m[1][3] + vec->z * m[2][3] + m[3][3]; 
	return (create_point(a / w, b / w, c / w));
}