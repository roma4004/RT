/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:41:21 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/16 20:21:20 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

t_dvec3		convert_to_viewport(double x, double y, double rate)
{
	return ((t_dvec3){x * (VIEWPORT_SIZE * rate) / WIN_WIDTH,
						y * VIEWPORT_SIZE / WIN_HEIGHT,
						DISTANCE_TO_PLANE, 0.0});
}

static void			put_px(t_env *env, const t_dvec *canvas_half,
							double x, double y, t_dvec3 *color)
{
	x = canvas_half->x + x;
	y = canvas_half->y - y - 1.0;
	if (x < 0.0 || x >= WIN_WIDTH
	|| y < 0.0 || y >= WIN_HEIGHT)
		return ;
	env->buff[(int)y][(int)(x)] = (((uint32_t)color->x) << 16u)
									| (((uint32_t)color->y) << 8u)
									| (((uint32_t)color->z));
}

static void			*render_frame(void *thread_data)
{
	t_env						*env = ((t_pth_dt *)thread_data)->env;
	register const size_t		thread_id = ((t_pth_dt *)thread_data)->id;
	const double				rate = env->cam.canvas.rate;
	const t_dvec				half = env->cam.canvas.half;
	t_ray						ray;
	t_dvec						pt;
	t_dvec3						color;

	ray = (t_ray){.t_min = env->cam.t_min,
		.t_max = env->cam.t_max,
		.pos = env->cam.pos,
		.dept_limit = env->cam.reflective_dept
	};

	ray.dir = convert_to_viewport(0, 0, rate);
	rotate_vec(&ray.dir, &env->cam.rotate_angle);
	env->origin_dir_z = ray.dir;
	vec3_normalize(&env->origin_dir_z, &env->origin_dir_z);

	ray.dir = convert_to_viewport(-half.x, 0, rate);
	rotate_vec(&ray.dir, &env->cam.rotate_angle);
	env->origin_dir_x = ray.dir;
	vec3_sub_vec3(&env->origin_dir_x, &env->origin_dir_x, &env->origin_dir_z);
	vec3_normalize(&env->origin_dir_x, &env->origin_dir_x);

	ray.dir = convert_to_viewport(0, -half.y, rate);
	rotate_vec(&ray.dir, &env->cam.rotate_angle);
	env->origin_dir_y = ray.dir;
	vec3_sub_vec3(&env->origin_dir_y, &env->origin_dir_y, &env->origin_dir_z);
	vec3_normalize(&env->origin_dir_y, &env->origin_dir_y);

	pt.y = -half.y - 1.0;
	while (++pt.y < half.y)
	{
		pt.x = -half.x - env->threads;
		while ((pt.x += env->threads) < half.x)
		{
			ray.dir = convert_to_viewport(pt.x + thread_id, pt.y, rate);
			rotate_vec(&ray.dir, &env->cam.rotate_angle);
			color = (t_dvec3){0, 0, 0};
			send_ray(env, &ray, &color);

			if (env->flags.is_sepia)
				apply_sepia_filter(&color);
			put_px(env, &half, pt.x + thread_id, pt.y, &color);
		}
	}
	return (NULL);
}

void				apply_sepia_filter(t_dvec3 *color)
{
	*color = (t_dvec3){
		.x = (color->x * 0.393) + (color->y * 0.769) + (color->z * 0.189),
		.y = (color->x * 0.349) + (color->y * 0.686) + (color->z * 0.168),
		.z = (color->x * 0.272) + (color->y * 0.534) + (color->z * 0.131)};
	ft_clamp_in_range_vec(color, 0, 255);
}

void				draw_scene(t_env *env, size_t threads)
{
	size_t		id;
	t_pth_dt	*data;
	pthread_t	*threads_arr;

	if (!env || !(data = (t_pth_dt *)malloc(threads * sizeof(t_pth_dt))))
		return ;
	if (!(threads_arr = (pthread_t *)malloc(threads * sizeof(pthread_t))))
	{
		free(data);
		return ;
	}
	id = UINT64_MAX;
	while (++id < threads)
	{
		data[id].env = env;
		data[id].id = id;
		pthread_create(&threads_arr[id], NULL, render_frame, &data[id]);
	}
	id = UINT64_MAX;
	while (++id < threads)
		pthread_join(threads_arr[id], NULL);
	SDL_UpdateTexture(env->screen, NULL, env->buff, WIN_WIDTH << 2u);
	SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);
	SDL_RenderPresent(env->renderer);
}

//сумму коеф зеркальности и прозрачностие должна быть меньше чем 0.98