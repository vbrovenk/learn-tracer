/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 17:30:32 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 17:30:34 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

int		general_options(char *line, t_shape *shape)
{
	if (ft_strnequ(line, "position: ", 10) == 1)
		read_position(line + 10, shape->center);
	else if (ft_strnequ(line, "color: ", 7) == 1)
	{
		shape->color = read_color(line + 7);
		if (shape->color < 20)
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
		if (is_number(line + 8) == 0 || shape->radius < 0)
			shape->radius = 1.0;
	}
	else if (ft_strnequ(line, "reflective: ", 12) == 1)
	{
		shape->reflective = read_double(line + 12);
	}
	else
		return (0);
	return (1);
}

void	read_direction(char *line, t_point *direction)
{
	char	**split;
	t_point	*norm;

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

void	check_hieght_cone(char *line, t_shape *shape, int nbr_height)
{
	if (nbr_height == 1)
	{
		shape->height_cone1 = read_double(line + 9);
		if (shape->height_cone1 < 0)
			shape->height_cone1 = INFINIT;
	}
	else if (nbr_height == 2)
	{
		shape->height_cone2 = read_double(line + 9);
		if (shape->height_cone2 < 0)
			shape->height_cone2 = INFINIT;
	}
}

void	read_cone(t_tracer *tracer, int fd)
{
	t_shape	*shape;
	char	*line;

	line = NULL;
	shape = create_shape(CONE);
	while (get_next_line(fd, &line) > 0)
	{
		if (general_options(line, shape) == 1)
			;
		else if (ft_strnequ(line, "direction: ", 11))
			read_direction(line + 11, shape->dir);
		else if (ft_strnequ(line, "height1: ", 9) == 1)
			check_hieght_cone(line, shape, 1);
		else if (ft_strnequ(line, "height2: ", 9) == 1)
			check_hieght_cone(line, shape, 2);
		else
			break ;
		ft_strdel(&line);
	}
	ft_strdel(&line);
	add_shape_to_list(&tracer->shapes, shape);
}

void	read_data(t_tracer *tracer, char *filename)
{
	int		fd;
	int		ret;
	char	*line;

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
