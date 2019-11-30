/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:41:21 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/28 14:35:10 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			convert_to_viewport(t_dvec3 *destination, const t_env *env,
					double x, double y)
{
	*destination = (t_dvec3){
		.x = x * (VIEWPORT_SIZE * env->cam.rate) / env->buff_width,
		.y = y * VIEWPORT_SIZE / env->buff_height,
		.z = DISTANCE_TO_PLANE};
	rotate_vec(destination, &env->cam.rotate_angle);
}

static void		put_px(t_env *env, const t_dvec *canvas_half,
					t_dvec pt, t_dvec3 *color)
{
	pt.x = canvas_half->x + pt.x;
	pt.y = canvas_half->y - pt.y - 1.0;
	if (pt.x < 0.0 || pt.x >= env->buff_width
	|| pt.y < 0.0 || pt.y >= env->buff_height)
		return ;
	apply_effects(&env->flags, color);
	env->buff[(Uint32)pt.y * env->buff_width + (Uint32)pt.x] =
		(((Uint32)color->x) << 16u)
		| (((Uint32)color->y) << 8u)
		| ((Uint32)color->z);
}

static void		save_orig_cam_dir(t_env *env, t_cam *cam, const t_dvec *half)
{
	convert_to_viewport(&cam->origin_dir_z, env, 0.0, 0.0);
	vec3_normalize(&cam->origin_dir_z, &cam->origin_dir_z);
	convert_to_viewport(&cam->origin_dir_x, env, -half->x, 0.0);
	vec3_sub_vec3(&cam->origin_dir_x, &cam->origin_dir_x, &cam->origin_dir_z);
	vec3_normalize(&cam->origin_dir_x, &cam->origin_dir_x);
	convert_to_viewport(&cam->origin_dir_y, env, 0.0, -half->y);
	vec3_sub_vec3(&cam->origin_dir_y, &cam->origin_dir_y, &cam->origin_dir_z);
	vec3_normalize(&cam->origin_dir_y, &cam->origin_dir_y);
}

static void		*render_frame(void *thread_data)
{
	const uint64_t		thread_id = ((t_pth_dt *)thread_data)->id;
	t_env				*env;
	t_ray				ray;
	t_dvec				pt;
	t_dvec3				color;

	env = ((t_pth_dt *)thread_data)->env;
	ray = (t_ray){.t_min = env->cam.t_min, .t_max = env->cam.t_max,
		.pos = env->cam.pos, .dept_limit = env->cam.reflective_dept};
	save_orig_cam_dir(env, &env->cam, &env->cam.half);
	pt.y = -env->cam.half.y - 1.0;
	while (++pt.y < env->cam.half.y)
	{
		pt.x = -env->cam.half.x - env->threads;
		while ((pt.x += env->threads) < env->cam.half.x)
		{
			convert_to_viewport(&ray.dir, env, pt.x + thread_id, pt.y);
			color = (t_dvec3){0.0, 0.0, 0.0, 0.0};
			send_ray(&color, env, &ray);
			put_px(env, &env->cam.half, (t_dvec){pt.x + thread_id, pt.y},
				&color);
		}
	}
	return (NULL);
}

void			draw_scene(t_env *env, uint64_t threads)
{
	uint64_t	id;
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
		data[id] = (t_pth_dt){.env = env, .id = id};
		pthread_create(&threads_arr[id], NULL, render_frame, &data[id]);
	}
	id = UINT64_MAX;
	while (++id < threads)
		pthread_join(threads_arr[id], NULL);
	screen_update(env);
	free(data);
	free(threads_arr);
}
