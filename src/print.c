#include "raytray.h"

void	info_about_point(t_point *point)
{
	printf("================ POINT =======================\n");
	printf("point->x = %f\n", point->x);
	printf("point->y = %f\n", point->y);
	printf("point->z = %f\n", point->z);
	printf("point->color = %d\n", point->color);
}

void	info_about_light(t_light *light)
{
	printf("================= LIGHT ======================\n");
	printf("light->position->x = %f\n", light->position->x);
	printf("light->position->y = %f\n", light->position->y);
	printf("light->position->z = %f\n", light->position->z);
	printf("light->type = %d\n", light->type);
	printf("light->intensity = %f\n", light->intensity);
}

void	info_about_shape(t_shape *shape)
{
	printf("================== SHAPE =====================\n");
	printf("shape->type = %d\n", shape->type);
	printf("shape->center = %f | %f | %f\n", shape->center->x, shape->center->y, shape->center->z);
	printf("shape->radius = %f\n", shape->radius);
	printf("shape->color = %d\n", shape->color);
	printf("shape->height_cylinder = %f\n", shape->height_cylinder);
	printf("shape->angle = %f\n", shape->angle);
	printf("shape->height_cone1 = %f\n", shape->height_cone1);
	printf("shape->height_cone2 = %f\n", shape->height_cone2);
	printf("shape->normal = %f | %f | %f\n", shape->dir->x, shape->dir->y, shape->dir->z);
	printf("shape->specular = %f\n", shape->specular);
	printf("shape->reflective = %f\n", shape->reflective);
}

void	print_list_lights(t_light *head)
{
	t_light *current;

	current = head;
	while (current != NULL)
	{
		info_about_light(current);
		current = current->next;
	}
}

void	print_list_shapes(t_shape *head)
{
	t_shape *current;

	current = head;
	while (current != NULL)
	{
		info_about_shape(current);
		current = current->next;
	}
}


void	info_about_matrix4x4(double (*matrix)[4])
{
	int i;
	int j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			printf("%f ", matrix[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}
