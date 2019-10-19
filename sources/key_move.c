/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 13:33:23 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 17:54:42 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		vec3_move(t_dvec3 *move_pos, const t_dvec3 *move_dir,
					double move_speed)
{
	t_dvec3		offset;

	vec3_mul_double(&offset, move_dir, move_speed);
	vec3_add_vec3(move_pos, move_pos, &offset);
}

void			move_objects(t_env *env, const t_dvec3 *move_dir,
					double move_speed)
{
	t_uni		*obj;
	size_t		i;

	if (move_speed != 0.0)
	{
		i = UINT64_MAX;
		while (++i < env->uni_arr_len && (obj = &env->uni_arr[i]))
			if (obj->is_selected)
				vec3_move(&obj->pos, move_dir, move_speed);
	}
}

void			move_camera(t_env *env, const t_dvec3 *move_dir,
					double move_speed)
{
	if (move_speed != 0.0)
		vec3_move(&env->cam.pos, move_dir, move_speed);
}
