/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 19:00:15 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/23 14:54:20 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		swith_handle(t_env *env, t_flags *flags, size_t obj_cnt)
{
	if (flags->is_reset)
		reset(env, &env->cam, obj_cnt);
	if (flags->is_screenshot)
		save_screenshot(env);
}

void		count_selected_obj(size_t *dest, t_uni *uni_arr, size_t uni_arr_len)
{
	size_t		obj_cnt;
	size_t		i;

	i = UINT64_MAX;
	obj_cnt = 0;
	while (++i < uni_arr_len)
		if (uni_arr[i].is_selected)
			obj_cnt++;
	*dest = obj_cnt;
}

_Bool		select_mod(t_env *env, const SDL_Event *event, const t_cam *cam)
{
	t_ray		ray;

	if (env->flags.is_in_select_mod
	&& event->button.button == SDL_BUTTON_LEFT)
	{
		ray = (t_ray){
			.t_min = cam->t_min,
			.t_max = cam->t_max,
			.pos = cam->pos,
			.dept_limit = cam->reflective_dept};
		convert_to_viewport(
			&ray.dir,
			env,
			event->motion.x - env->cam.half.x,
			-event->motion.y + env->cam.half.y - 1.0);
		send_selected_ray(&env->selected_obj, env, &ray, (double)MAXFLOAT);
		if (env->selected_obj)
		{
			env->selected_obj->is_selected =
				(env->selected_obj->is_selected) ? false : true;
			select_caps_cylinder_cone(env);
		}
		return (true);
	}
	return (false);
}
