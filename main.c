/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 16:57:51 by vbrovenk          #+#    #+#             */
/*   Updated: 2018/08/21 16:57:52 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytray.h"

void	init_struct(t_tracer *fdf)
{
	fdf->mlx_ptr = NULL;
	fdf->win_ptr = NULL;

}


int		x_exit(void *param)
{
	param = NULL;
	exit(1);
	return (0);
}

int choose_key(int key, void *tracer)
{
	if (key == ESC)
		x_exit(tracer);
	return (0);
}

void	draw_line(t_tracer *tracer, t_point p1, t_point p2)
{
	int dx = (p2.x - p1.x >= 0 ? 1 : -1);
	int dy = (p2.y - p1.y >= 0 ? 1 : -1);

	float lengthX = abs(p2.x - p1.x);
	float lengthY = abs(p2.y - p1.y);

	float length = fmax(lengthX, lengthY);

	if (length == 0)
		mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, p1.x, p1.y, 0x00FF00);
	if (lengthY <= lengthX)
	{
		float x = p1.x;
		float y = p1.y;
		float d = -lengthX;

		length++;
		while (length--)
		{
			mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, x, y, 0x00FF00);
			x += dx;
			d += 2 * lengthY;
			if (d > 0)
			{
				d -= 2 * lengthX;
				y += dy;
			}
		}
	}
	else
	{
		float x = p1.x;
		float y = p1.y;
		float d = -lengthY;

		length++;
		while (length--)
		{
			mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, x, y, 0x00FF00);
			y += dy;
			d += 2 * lengthX;
			if (d > 0)
			{
				d -= 2 * lengthY;
				x += dx;
			}
		}

	}
}


void convert_to_conterclock(t_point *corners)
{
	for (int i = 0; i < 8; i++)
	{
		corners[i].y *= -1;
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
		float x_proj_remap = (1 + x_proj) / 2;
		float y_proj_remap = (1 + y_proj) / 2;
		printf("remap corner: %d x:%f y:%f\n", i, x_proj_remap, y_proj_remap);


		float x_proj_pix = x_proj_remap * WIDTH;
		float y_proj_pix = y_proj_remap * HEIGHT;
		printf("in pixels: %d x:%f y:%f\n", i, x_proj_pix, y_proj_pix);

		mlx_pixel_put(tracer->mlx_ptr, tracer->win_ptr, 
			x_proj_pix, y_proj_pix, corners[i].color);		

		printf("\n");
	}
}

// compilation without FLAGS
int main(int argc, char const **argv)
{
	t_tracer	*tracer;

	tracer = (t_tracer*)malloc(sizeof(t_tracer));
	init_struct(tracer);
	tracer->mlx_ptr = mlx_init();
	tracer->win_ptr = mlx_new_window(tracer->mlx_ptr, WIDTH, HEIGHT, "TRACER");

	draw(tracer);

	mlx_hook(tracer->win_ptr, 2, 5, choose_key, tracer);
	mlx_loop(tracer->mlx_ptr);

	return (0);
}