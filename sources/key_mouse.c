/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_mouse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 21:59:58 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 22:01:31 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		select_caps_cylinder_cone(t_env *env)
{
	int i;

	i = -1;
	while (++i < env->uni_arr_len)
		if (env->selected_obj == &env->uni_arr[i])
		{
			if ((env->selected_obj->get_intersect == get_intersect_cylinder)
			|| (env->selected_obj->get_intersect == get_intersect_cone))
				env->uni_arr[i + 1].is_selected =
					env->selected_obj->is_selected;
			if (env->selected_obj->get_intersect == get_intersect_cylinder)
				env->uni_arr[i + 2].is_selected =
					env->selected_obj->is_selected;
			break;
		}
}