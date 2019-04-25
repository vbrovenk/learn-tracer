/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canvas.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 17:36:14 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/04/25 17:36:16 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

void	put_pixel(t_tracer *tracer, int x, int y, int color)
{
	int screen_x;
	int screen_y;

	screen_x = WIDTH / 2 + x;
	screen_y = HEIGHT / 2 - y - 1;
	if (screen_x < 0 || screen_x >= WIDTH || screen_y < 0 || screen_y >= HEIGHT)
		return ;
	tracer->image[screen_x + screen_y * HEIGHT] = color;
}

t_point	*canvas_to_viewport(int x, int y)
{
	t_point	*view_point;
	double	a;
	double	b;

	a = x * VIEWPORT_SIZE / (double)WIDTH;
	b = y * VIEWPORT_SIZE / (double)HEIGHT;
	view_point = create_point(a, b, PROJECTION_PLANE_Z);
	return (view_point);
}

int		mult_k_color(double k, int color)
{
	int r;
	int g;
	int b;

	r = (0xFF0000 & color) >> 16;
	g = (0x00FF00 & color) >> 8;
	b = (0x0000FF & color);
	r *= k;
	g *= k;
	b *= k;
	// Clamps a color to the canonical color range.
	r = fmin(255, fmax(0, r));
	g = fmin(255, fmax(0, g));
	b = fmin(255, fmax(0, b));
	return ((r << 16) | (g << 8) | (b));
}
