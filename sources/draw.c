/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:41:21 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 18:06:19 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static t_dvec3		convert_to_viewport(t_dvec pt, double rate)
{
	return ((t_dvec3){ pt.x * (VIEWPORT_SIZE * rate) / WIN_WIDTH,
						pt.y * VIEWPORT_SIZE / WIN_HEIGHT,
						DISTANCE_TO_PLANE });
}

static void			put_px(t_env *env, const t_dvec *canvas_half,
							t_dvec pt, t_dvec3 *color)
{
	pt.x += canvas_half->x;
	pt.y = canvas_half->y - pt.y - 1;
	if (pt.x < 0 || pt.x >= WIN_WIDTH
	|| pt.y < 0 || pt.y >= WIN_HEIGHT)
		return ;
	env->buff[(int)pt.y][(int)pt.x] = (((uint32_t)color->x) << 16u)
									| (((uint32_t)color->y) << 8u)
									| (((uint32_t)color->z));
}

void				rerender_scene(t_env *env)
{
	const t_dvec	half = env->cam.canvas.half;
	const double	rate = env->cam.canvas.rate;
	t_dvec			pt;
	t_dvec3			color;

	pt.y = -half.y - 1;
	while (++pt.y < half.y)
	{
		pt.x = -half.x - 1;
		while (++pt.x < half.x)
		{
			env->cam.dir = convert_to_viewport(pt, rate);
			rotate_cam(&env->cam.dir, &env->cam.rotate_angle);
			send_ray(env, &env->cam, &color);
			put_px(env, &half, pt, &color);
		}
	}
	SDL_UpdateTexture(env->screen, NULL, env->buff, WIN_WIDTH << 2u);
	SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);
	SDL_RenderPresent(env->renderer);
}
