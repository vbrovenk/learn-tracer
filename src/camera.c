/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 18:25:11 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 18:25:13 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

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

void	read_position(char *line, t_point *point)
{
	char **split;

	split = ft_strsplit(line, ',');
	if (count_splits(split) == 3)
	{
		point->x = read_double(split[0]);
		point->y = read_double(split[1]);
		point->z = read_double(split[2]);
	}
	clear_split(split);
}

double	read_double(char *line)
{
	double	nbr;
	char	*point;
	int		minus;
	char	*trim;
	int		i;

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
	if (point != NULL && ft_isdigit(point[1]) == 0)
		return (0);
	if (point != NULL)
		nbr += ft_atoi(point + 1) / pow(10, ft_strlen(point + 1));
	ft_strdel(&trim);
	return (nbr * minus);
}

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
