/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:41:21 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/24 14:32:00 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static uint8_t  double2byte_clamp(const double x)
{
	if (x < 0.0)
		return (0);
	else if (x > 1.0)
		return (255);
	else
		return ((uint8_t)(255.0 * x));
}

uint32_t		vec3_to_color(const t_dvec3 *restrict first)
{
	return (  double2byte_clamp(first->x) << (1u * 8u)
			| double2byte_clamp(first->y) << (2u * 8u)
			| double2byte_clamp(first->z) << (3u * 8u));
}

static void		put_px(t_env *env, double x, double y, t_dvec3 color)
{
	x = env->canvas_half.x + x;
	y = env->canvas_half.y - y - 1;
	if (x < 0 || x >= WIN_WIDTH
	|| y < 0 || y >= WIN_HEIGHT)
		return ;
	env->buff[(int)y][(int)x] = ( 255u             << 24u)
							| (((uint32_t)color.x) << 16u)
							| (((uint32_t)color.y) << 8u)
							| (((uint32_t)color.z));
}

static void		clear_img_buff(t_env *env)
{
	Uint32	y;
	Uint32	x;

	if (!env)
		return ;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
			env->buff[y][x++] = 0x0;
		y++;
	}
}

static int		prepare_render(t_env *env, t_fps *fps)
{
	fps->pre_tick = fps->cur_tick;
	fps->cur_tick = SDL_GetTicks();
	fps->frame_time = (fps->cur_tick - fps->pre_tick) / 1000.0;
	fps->value = (u_char)(1.0 / fps->frame_time);
//	env->cam.move_speed = fps->frame_time * 3.0;
//	env->cam.rotate_speed = fps->frame_time * 2.0;
//	if ((fps->frame_limit_second) > fps->cur_tick - fps->pre_tick)
//		SDL_Delay(fps->frame_limit_second - (fps->cur_tick - fps->pre_tick));
	return (1);
}

void			rerender_scene(t_env *env)
{
	double		y;
	double		x;

	while (!env->flags.is_rtv1_over && prepare_render(env, &env->fps))
	{
		clear_img_buff(env);
		y = -env->canvas_half.y - 1;
		while (++y < env->canvas_half.y)
		{
			x = -env->canvas_half.x - 1;
			while (++x < env->canvas_half.x)
			{
				env->cam.dir = convert_to_viewport(x, y);
				rotate_cam(env);
				t_dvec3 color = send_ray(env);//(t_fvec3){0};
				put_px(env, x, y, color);
			}
		}

		SDL_UpdateTexture(env->screen, NULL, env->buff, WIN_WIDTH << 2u);
		SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);

		SDL_RenderPresent(env->renderer);
		event_handler(env, &env->cam, &env->flags);
	}
}
