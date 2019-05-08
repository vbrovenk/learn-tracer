/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 17:30:21 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 17:30:23 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

t_point	*rotation_x(t_point *old, int degrees)
{
	t_point	*new;

	new = create_point(old->x, old->y, old->z);
	new->y = old->y * cos(degrees * M_PI / 180) -
										old->z * sin(degrees * M_PI / 180);
	new->z = old->y * sin(degrees * M_PI / 180) +
										old->z * cos(degrees * M_PI / 180);
	return (new);
}

t_point	*rotation_y(t_point *old, int degrees)
{
	t_point	*new;

	new = create_point(old->x, old->y, old->z);
	new->x = old->x * cos(degrees * M_PI / 180) +
										old->z * sin(degrees * M_PI / 180);
	new->z = -old->x * sin(degrees * M_PI / 180) +
										old->z * cos(degrees * M_PI / 180);
	return (new);
}
