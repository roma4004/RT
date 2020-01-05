/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 18:21:50 by dromanic          #+#    #+#             */
/*   Updated: 2020/01/05 21:08:41 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

static void		thread_render_buffer(t_env *env, size_t thread_id)
{
	t_dvec				pt;
	t_ray				ray;
	t_dvec3				color;

	ray = (t_ray){
		.t_min = env->cam.t_min,
		.t_max = env->cam.t_max,
		.pos = env->cam.pos,
		.dept_limit = env->cam.reflective_dept};
	save_orig_cam_dir(env, &env->cam, &env->cam.half);
	pt.y = -env->cam.half.y - 1.0;
	while (++pt.y < env->cam.half.y)
	{
		pt.x = -env->cam.half.x - env->threads;
		while ((pt.x += env->threads) < env->cam.half.x)
		{
			convert_to_viewport(&ray.dir, env, pt.x + thread_id, pt.y);
			color = (t_dvec3) {0.0, 0.0, 0.0, 0.0};
			send_ray(&color, env, &ray);
			put_px(env, &env->cam.half,
				(t_dvec){pt.x + thread_id, pt.y}, &color);
		}
	}
	--(env->working_threads_amount);
	env->thread_status[thread_id] = false;
}

void			*render_frame(void *thread_data)
{
	const size_t		thread_id = ((t_pth_dt *)thread_data)->id;
	t_env				*env;
//	pthread_mutex_t		count_lock;

	env = ((t_pth_dt *)thread_data)->env;
	while (!(env->flags.is_rtv1_running))
	{
		pthread_mutex_lock(env->count_lock + thread_id);


		//cond_wait test section:
		while (!(*(env->thread_status + thread_id)))
			pthread_cond_wait(&env->buffer_obsolete, env->count_lock + thread_id);
		thread_render_buffer(env, thread_id);

		//original sleep section:
//		if (!(*(env->thread_status + thread_id)))
//			usleep(100);
//		else
//			thread_render_buffer(env, thread_id);

		pthread_mutex_unlock(env->count_lock + thread_id);
	}
	return (NULL);
}
