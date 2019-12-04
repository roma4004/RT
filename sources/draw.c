/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:41:21 by dromanic          #+#    #+#             */
/*   Updated: 2019/12/04 12:11:08 by dromanic         ###   ########.fr       */
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
	uint64_t	status;

	i = UINT64_MAX;
	while (++i < env->threads)
		env->thread_status[i] = true;
	while (true)
	{
		status = 0;
		i = UINT64_MAX;
		while (++i < env->threads)
			status += env->thread_status[i];
		if (status == 0)
			break ;
		else
			screen_update(env);
	}
	screen_update(env);
}
