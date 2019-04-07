#include "raytray.h"

void	init_rotation(t_tracer *tracer)
{
	int i;
	int j;

	tracer->camera_rotation = (t_m3x3 *)malloc(sizeof(t_m3x3));
	i = -1;
	while (++i < 3)
	{
		j = -1;
		while (++j < 3)
			tracer->camera_rotation->matrix[i][j] = 0;
	}
	tracer->camera_rotation->matrix[0][0] = 1;
	tracer->camera_rotation->matrix[1][1] = 1;
	tracer->camera_rotation->matrix[2][2] = 1;
}

void	rotation_x(t_tracer *tracer)
{
	tracer->camera_rotation->matrix[1][1] = cos(tracer->degrees_x * M_PI / 180);
	tracer->camera_rotation->matrix[1][2] = -sin(tracer->degrees_x * M_PI / 180);
	tracer->camera_rotation->matrix[2][1] = sin(tracer->degrees_x * M_PI / 180);
	tracer->camera_rotation->matrix[2][2] = cos(tracer->degrees_x * M_PI / 180);


}
