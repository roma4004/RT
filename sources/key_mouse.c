/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_mouse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 21:59:58 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 17:54:42 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	select_caps_cylinder_cone(t_env *env)
{
	size_t		i;

	i = UINT64_MAX;
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
			break ;
		}
}
