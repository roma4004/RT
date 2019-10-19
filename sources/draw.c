/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:41:21 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 18:57:38 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_dvec3			convert_to_viewport(double x, double y, double rate)
{
	return ((t_dvec3){
		.x = x * (VIEWPORT_SIZE * rate) / WIN_WIDTH,
		.y = y * VIEWPORT_SIZE / WIN_HEIGHT,
		.z = DISTANCE_TO_PLANE});
}

static void		put_px(t_env *env, const t_dvec *canvas_half,
					t_dvec pt, t_dvec3 *color)
{
	pt.x = canvas_half->x + pt.x;
	pt.y = canvas_half->y - pt.y - 1.0;
	if (pt.x < 0.0 || pt.x >= WIN_WIDTH
	|| pt.y < 0.0 || pt.y >= WIN_HEIGHT)
		return ;
	env->buff[(unsigned)pt.y][(unsigned)pt.x] =
		(((uint32_t)color->x) << 16u)
		| (((uint32_t)color->y) << 8u)
		| ((uint32_t)color->z);
}

static void		save_orig_cam_dir(t_cam *cam, const t_dvec *half)
{
	cam->origin_dir_z = convert_to_viewport(0.0, 0.0, cam->rate);
	rotate_vec(&cam->origin_dir_z, &cam->rotate_angle);
	vec3_normalize(&cam->origin_dir_z, &cam->origin_dir_z);
	cam->origin_dir_x = convert_to_viewport(-half->x, 0.0, cam->rate);
	rotate_vec(&cam->origin_dir_x, &cam->rotate_angle);
	vec3_sub_vec3(&cam->origin_dir_x, &cam->origin_dir_x, &cam->origin_dir_z);
	vec3_normalize(&cam->origin_dir_x, &cam->origin_dir_x);
	cam->origin_dir_y = convert_to_viewport(0.0, -half->y, cam->rate);
	rotate_vec(&cam->origin_dir_y, &cam->rotate_angle);
	vec3_sub_vec3(&cam->origin_dir_y, &cam->origin_dir_y, &cam->origin_dir_z);
	vec3_normalize(&cam->origin_dir_y, &cam->origin_dir_y);
}

static void		*render_frame(void *thread_data)
{
	t_env		*env;
	size_t		thread_id;
	t_cam		*cam;
	t_dvec		half;
	t_ray		ray;
	t_dvec		pt;
	t_dvec3		color;

	env = ((t_pth_dt *)thread_data)->env;
	thread_id = ((t_pth_dt *)thread_data)->id;
	cam = &env->cam;
	half = cam->half;
	ray = (t_ray){.t_min = cam->t_min,
		.t_max = cam->t_max,
		.pos = cam->pos,
		.dept_limit = cam->reflective_dept
	};
	save_orig_cam_dir(cam, &half);
	pt.y = -half.y - 1.0;
	while (++pt.y < half.y)
	{
		pt.x = -half.x - env->threads;
		while ((pt.x += env->threads) < half.x)
		{
			ray.dir =
				convert_to_viewport(pt.x + thread_id, pt.y, cam->rate);
			rotate_vec(&ray.dir, &cam->rotate_angle);
			color = (t_dvec3){0, 0, 0};
			send_ray(env, &ray, &color);
			apply_effects(&env->flags, &color);
			put_px(env, &half, (t_dvec){pt.x + thread_id, pt.y}, &color);
		}
	}
	return (NULL);
}

void			draw_scene(t_env *env, size_t threads)
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
		data[id] = (t_pth_dt){.env = env, .id = id};
		pthread_create(&threads_arr[id], NULL, render_frame, &data[id]);
	}
	id = UINT64_MAX;
	while (++id < threads)
		pthread_join(threads_arr[id], NULL);
	SDL_UpdateTexture(env->screen, NULL, env->buff, WIN_WIDTH << 2u);
	SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);
	SDL_RenderPresent(env->renderer);
	free(data);
	free(threads_arr);
}
