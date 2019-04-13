#include "raytray.h"

t_shape *create_shape(int type, t_point *center, double radius, int color,
										double specular, double reflective)
{
	t_shape *shape;

	shape = (t_shape *)malloc(sizeof(t_shape));
	shape->type = type;

	shape->center = center;
	shape->radius = radius;
	shape->color = color;

	shape->specular = specular;
	shape->reflective = reflective;
	shape->next = NULL;
	return (shape);
}


t_sphere *init_sphere(t_point *center, double radius, int color,
										double specular, double reflective)
{
	t_sphere *sphere;

	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	sphere->center = center;
	sphere->radius = radius;
	sphere->color = color;

	sphere->specular = specular;
	sphere->reflective = reflective;
	sphere->next = NULL;
	return (sphere);
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

void	add_sphere_to_list(t_sphere **head, t_sphere *sphere)
{
	t_sphere *current;

	current = *head;
	if (current == NULL)
	{
		*head = sphere;
	}
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = sphere;
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
