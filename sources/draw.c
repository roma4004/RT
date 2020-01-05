/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:41:21 by dromanic          #+#    #+#             */
/*   Updated: 2020/01/05 20:07:29 by dromanic         ###   ########.fr       */
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

void			draw_scene(t_env *env)
{
	uint64_t	i;

	env->working_threads_amount = env->threads;
	i = UINT64_MAX;
	while (++i < env->threads)
		env->thread_status[i] = true;
	pthread_cond_broadcast(&env->buffer_obsolete);
	while (true)
		if (env->working_threads_amount)
			screen_update(env);
		else
			break ;
	screen_update(env);
}
