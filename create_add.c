#include "raytray.h"

t_shape *create_shape(int type, t_point *center, double radius, int color, double height_cylinder,
										double specular, double reflective)
{
	t_shape *shape;

	shape = (t_shape *)malloc(sizeof(t_shape));
	shape->type = type;

	shape->center = center;
	shape->radius = radius;
	shape->color = color;

	shape->height_cylinder = height_cylinder;

	shape->specular = specular;
	shape->reflective = reflective;
	shape->next = NULL;
	return (shape);
}

t_shape *create_cone(int type, t_point *center, double radius, int color, double height_cylinder,
						double angle, double height_cone1, double height_cone2,
										double specular, double reflective)
{
	t_shape *shape;

	shape = (t_shape *)malloc(sizeof(t_shape));
	shape->type = type;

	shape->center = center;
	shape->radius = radius;
	shape->color = color;

	shape->height_cylinder = height_cylinder;

	// for cone
	shape->angle = tan(deg_to_rad(60));
	shape->height_cone1 = height_cone1;
	shape->height_cone2 = height_cone2;

	shape->specular = specular;
	shape->reflective = reflective;
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
	return(new_light);
}

void	add_shape_to_list(t_shape **head, t_shape *shape)
{
	t_shape *current;

	current = *head;
	if (current == NULL)
	{
		*head = shape;
	}
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
	{
		*head = light;
	}
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = light;
	}
}
