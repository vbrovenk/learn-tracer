#include "raytray.h"

int		count_splits(char **split)
{
	int count;

	count = 0;
	while (split[count] != NULL)
		count++;
	return (count);
}

void	clear_split(char **split)
{
	int i;

	i = 0;
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

double	read_double(char *line)
{
	double nbr;
	char *point;
	int minus;
	char *trim;
	int i;

	i = 0;
	trim = ft_strtrim(line);
	minus = 1;
	if (trim[0] == '-')
	{
		minus = -1;
		i += 1;
	}
	nbr = ft_atoi(&trim[i]);
	point = ft_strchr(&trim[i], '.');
	if (point != NULL)
		nbr += ft_atoi(point + 1) / pow(10, ft_strlen(point + 1));
	ft_strdel(&trim);
	printf("nbr = %f\n", nbr * minus);
	return (nbr * minus);
}

// void	read_position(char *line, t_point *point)
// {
// 	char **split;

// 	split = ft_strsplit(line, ',');
// 	if (count_splits(split) == 3)
// 	{
// 		point->x = read_double(split[0]);
// 		point->y = read_double(split[1]);
// 		point->z = read_double(split[0]);
// 	}
// 	clear_split(split);
// }

void	read_position(char *line, t_point *point)
{
	char **split;
	char *temp;

	split = ft_strsplit(line, ',');
	if (count_splits(split) == 3)
	{
		temp = ft_strtrim(split[0]);
		if (ft_isinteger(temp) == 0)
			print_error("Not integer");
		point->x = ft_atoi(temp);
		free(temp);
		temp = ft_strtrim(split[1]);
		if (ft_isinteger(temp) == 0)
			print_error("Not integer");
		point->y = ft_atoi(temp);
		free(temp);
		temp = ft_strtrim(split[2]);
		if (ft_isinteger(temp) == 0)
			print_error("Not integer");
		point->z = ft_atoi(temp);
		free(temp);
	}
	clear_split(split);
}

void	read_camera(t_tracer *tracer, int fd)
{
	char *line;

	if (tracer->camera_position != NULL)
		print_error("Camera already exists");
	tracer->camera_position = create_point(0, 0, 0);
	get_next_line(fd, &line);
	if (ft_strnequ(line, "position: ", 10) == 0)
		print_error("Error with position");
	read_position(line + 10, tracer->camera_position);
	ft_strdel(&line);
}

double	read_intensity(char *line)
{
	double nbr;
	char *point;

	nbr = ft_atoi(line);
	point = ft_strchr(line, '.');
	if (point != NULL)
		nbr += ft_atoi(point + 1) / pow(10, ft_strlen(point + 1));
	if (nbr < 0.0)
		return (0.0);
	if (nbr > 1.0)
		return (1.0);
	return (nbr);

}

void	read_light(t_tracer *tracer, int fd)
{
	t_light *p_light;
	char *line;

	p_light = create_light(create_point(0, 0, 0), POINT, 0.5);
	get_next_line(fd, &line);
	if (ft_strnequ(line, "position: ", 10) == 0)
		print_error("Error with light's position");
	read_position(line + 10, p_light->position);
	ft_strdel(&line);
	get_next_line(fd, &line);
	if (ft_strnequ(line, "intensity: ", 11) == 0)
		print_error("Error with light's intensity");
	p_light->intensity = read_intensity(line + 11);
	add_light_to_list(&tracer->lights, p_light);
	ft_strdel(&line);	
}

t_shape	*create_shape(int type)
{
	t_shape *shape;

	shape = (t_shape *)malloc(sizeof(t_shape));
	shape->type = type;
	shape->center = create_point(0, 0, 3);
	shape->radius = 1.0;
	shape->color = 0x00FF00;
	shape->height_cylinder = INFINIT;
	shape->angle = tan(deg_to_rad(20));
	shape->height_cone1 = INFINIT;
	shape->height_cone2 = INFINIT;
	shape->dir = create_point(0, 1, 0);
	shape->specular = 500;
	// shape->reflective = 0.5;
	shape->next = NULL;
	return (shape);
}

int		is_number(char *line)
{
	int i;
	int	count_dots;

	i = 0;
	count_dots = 0;
	while (line[i])
	{
		if (ft_isdigit(line[i]) || line[i] == '-')
			i++;
		else if (line[i] == '.')
		{
			i++;
			count_dots++;
		}
		else
			return (0);
	}
	if (count_dots > 1)
		return (0);
	return (1);
}

int		read_color(char *line)
{
	char **split;
	int		rgb;
	int		i;
	int		color;
	int		shift_left;

	shift_left = 16;
	i = 0;
	color = 0;
	split = ft_strsplit(line, ',');
	if (count_splits(split) == 3)
		while (i < 3)
		{
			rgb = ft_atoi(split[i]);
			if (rgb > 255 || rgb < 0)
				rgb = 255;
			color = color | (rgb << shift_left);
			shift_left -= 8;
			i++;
		}
	clear_split(split);
	return (color);
}

int		general_options(char *line, t_shape *shape)
{
	if (ft_strnequ(line, "position: ", 10) == 1)
		read_position(line + 10, shape->center);
	else if (ft_strnequ(line, "color: ", 7) == 1)
	{
		shape->color = read_color(line + 7);
		if (shape->color == 0)
			shape->color = 0x00FF00;
	}
	else if (ft_strnequ(line, "specular: ", 10) == 1)
	{
		shape->specular = read_double(line + 10);
		if (shape->specular < 0)
			shape->specular = 100;
	}
	else if (ft_strnequ(line, "radius: ", 8) == 1)
	{
		shape->radius = read_double(line + 8);
		if(is_number(line + 8) == 0 || shape->radius < 0)
			shape->radius = 1.0;
	}
	else
		return (0);
	return (1);
}

void	read_sphere(t_tracer *tracer, int fd)
{
	t_shape *shape;
	char *line;

	shape = create_shape(SPHERE);
	line = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		if (general_options(line, shape) == 1)
			;
		else
			break ;
		ft_strdel(&line);
	}
	ft_strdel(&line);
	add_shape_to_list(&tracer->shapes, shape);
}

void	read_direction_plane(char *line, t_point *direction)
{
	char **split;
	t_point *norm;

	split = ft_strsplit(line, ',');
	if (count_splits(split) == 3)
	{
		direction->x = read_double(split[0]);
		if (direction->x < 0)
			direction->x *= -1;
		direction->y = read_double(split[1]);
		if (direction->y < 0)
			direction->y *= -1;
		direction->z = read_double(split[2]);
		if (direction->z > 0)
			print_error("Direction's z must be < 0");
	}
	norm = normalize(direction);
	direction->x = norm->x;
	direction->y = norm->y;
	direction->z = norm->z;
	free(norm);
	clear_split(split);
}

void	read_direction(char *line, t_point *direction)
{
	char **split;
	t_point *norm;

	split = ft_strsplit(line, ',');
	if (count_splits(split) == 3)
	{
		direction->x = read_double(split[0]);
		direction->y = read_double(split[1]);
		direction->z = read_double(split[2]);
	}
	norm = normalize(direction);
	direction->x = norm->x;
	direction->y = norm->y;
	direction->z = norm->z;
	free(norm);
	clear_split(split);
}

void	read_plane(t_tracer *tracer, int fd)
{
	t_shape *shape;
	char *line;

	line = NULL;
	shape = create_shape(PLANE);
	while (get_next_line(fd, &line) > 0)
	{
		if (general_options(line, shape) == 1)
			;
		else if (ft_strnequ(line, "direction: ", 11))
			read_direction_plane(line + 11, shape->dir);
		else
			break ;
		ft_strdel(&line);
	}
	ft_strdel(&line);
	add_shape_to_list(&tracer->shapes, shape);
}

void	read_cylinder(t_tracer *tracer, int fd)
{
	t_shape *shape;
	char *line;

	line = NULL;
	shape = create_shape(CYLINDER);
	while (get_next_line(fd, &line) > 0)
	{
		if (general_options(line, shape) == 1)
			;
		else if (ft_strnequ(line, "direction: ", 11))
			read_direction(line + 11, shape->dir);
		else if (ft_strnequ(line, "height: ", 8) == 1)
		{
			shape->height_cylinder = read_double(line + 8);
			if (shape->height_cylinder <= 0)
				shape->height_cylinder = INFINIT;
		}
		else
			break ;
		ft_strdel(&line);
	}
	ft_strdel(&line);
	add_shape_to_list(&tracer->shapes, shape);
}

void	read_cone(t_tracer *tracer, int fd)
{
	t_shape *shape;
	char *line;

	line = NULL;
	shape = create_shape(CONE);
	while (get_next_line(fd, &line) > 0)
	{
		if (general_options(line, shape) == 1)
			;
		else if (ft_strnequ(line, "direction: ", 11))
			read_direction(line + 11, shape->dir);
		else if (ft_strnequ(line, "height1: ", 9) == 1)
		{
			shape->height_cone1 = read_double(line + 9);
			if (shape->height_cone1 < 0)
				shape->height_cone1 = INFINIT;
		}
		else if (ft_strnequ(line, "height2: ", 9) == 1)
		{
			shape->height_cone2 = read_double(line + 9);
			if (shape->height_cone2 < 0)
				shape->height_cone2 = INFINIT;
		}
		else
			break ;
		ft_strdel(&line);
	}
	ft_strdel(&line);
	add_shape_to_list(&tracer->shapes, shape);
}

void	read_data(t_tracer *tracer, char *filename)
{
	int fd;
	int ret;
	char *line;

	if ((fd = open(filename, O_RDONLY)) == -1)
		print_error("Can't open this file");
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		if (ft_strequ(line, "CAMERA") == 1)
			read_camera(tracer, fd);
		else if (ft_strequ(line, "LIGHT") == 1)
			read_light(tracer, fd);
		else if (ft_strequ(line, "SPHERE") == 1)
			read_sphere(tracer, fd);
		else if (ft_strequ(line, "PLANE") == 1)
			read_plane(tracer, fd);
		else if (ft_strequ(line, "CYLINDER") == 1)
			read_cylinder(tracer, fd);
		else if (ft_strequ(line, "CONE") == 1)
			read_cone(tracer, fd);
		ft_strdel(&line);
	}
}
