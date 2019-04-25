/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_light.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 18:34:36 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 18:34:37 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

double	read_intensity(char *line)
{
	double	nbr;
	char	*point;

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
	t_light	*p_light;
	char	*line;

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
	char	**split;
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
