/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 17:24:02 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 17:24:04 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

t_shape	*create_shape(int type)
{
	t_shape *shape;

	shape = (t_shape *)malloc(sizeof(t_shape));
	shape->type = type;
	shape->center = create_point(0, 0, 3);
	shape->radius = 1.0;
	shape->color = 0x00FF00;
	shape->height_cylinder = INFINIT;
	shape->angle = tan(20 * M_PI / 180);
	shape->height_cone1 = INFINIT;
	shape->height_cone2 = INFINIT;
	shape->dir = create_point(0, 1, 0);
	shape->specular = 500;
	shape->next = NULL;
	return (shape);
}

t_light	*create_light(t_point *position, int type, double intensity)
{
	t_light *new_light;

	new_light = (t_light *)malloc(sizeof(t_light));
	new_light->position = position;
	new_light->type = type;
	new_light->intensity = intensity;
	new_light->next = NULL;
	return (new_light);
}

void	add_shape_to_list(t_shape **head, t_shape *shape)
{
	t_shape *current;

	current = *head;
	if (current == NULL)
		*head = shape;
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = shape;
	}
}

void	add_light_to_list(t_light **head, t_light *light)
{
	t_light *current;

	current = *head;
	if (current == NULL)
		*head = light;
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = light;
	}
}
