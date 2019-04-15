#include "raytray.h"

void	info_about_point(t_point *point)
{
	printf("=======================================\n");
	printf("point->x = %f\n", point->x);
	printf("point->y = %f\n", point->y);
	printf("point->z = %f\n", point->z);
	printf("point->color = %d\n", point->color);
}

void	info_about_light(t_light *light)
{
	printf("=======================================\n");
	printf("light->position->x = %f\n", light->position->x);
	printf("light->position->y = %f\n", light->position->y);
	printf("light->position->z = %f\n", light->position->z);
	printf("light->type = %d\n", light->type);
	printf("light->intensity = %f\n", light->intensity);
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

void draw(t_tracer *tracer)
{
	t_point corners[8] = {
		{ 1, -1, -5, 0xFF0000},
		{ 1, -1, -3, 0x00FF00},
		{ 1,  1, -5, 0x0000FF},
		{ 1,  1, -3, 0xFFFFFF},
		
		{-1, -1, -5, 0xFFFFFF},
		{-1, -1, -3, 0xFFFFFF},
		{-1,  1, -5, 0xFFFFFF},
		{-1,  1, -3, 0xFFFFFF} 
	};


	// convert_to_conterclock(corners);
	int color = 0xFF0000;

	// get object's points on the canvas
	for (int i = 0; i < 8; i++)
	{
		// divide the x and y coordinates by the z coordinate to 
		// project the point on the canvas
		double x_proj = corners[i].x / -corners[i].z;
		double y_proj = corners[i].y / -corners[i].z;
		printf("projected corner: %d x:%f y:%f\n", i, x_proj, y_proj);

		// normalize points to range [0, 1]
		double x_proj_remap = (1 + x_proj) / 2;
		double y_proj_remap = (1 + y_proj) / 2;
		printf("remap corner: %d x:%f y:%f\n", i, x_proj_remap, y_proj_remap);


		double x_proj_pix = x_proj_remap * WIDTH;
		double y_proj_pix = y_proj_remap * HEIGHT;
		printf("in pixels: %d x:%f y:%f\n", i, x_proj_pix, y_proj_pix);

		mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, 
			x_proj_pix, y_proj_pix, corners[i].color);		

		printf("\n");
	}
}