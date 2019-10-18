/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 13:33:23 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:33:20 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		vec3_move(t_dvec3 *move_pos, t_env *env,
	t_dvec3 *move_dir, double move_speed)
{
	t_dvec3		offset;

	vec3_mul_double(&offset, move_dir, move_speed);
	vec3_add_vec3(move_pos, move_pos, &offset);
}

void			move_objects(t_env *env, t_dvec3 *move_dir, double move_speed)
{
	size_t	i;

	i = ~0UL;
	while (++i < env->uni_arr_len)
		if (env->uni_arr[i].is_selected)
			vec3_move(&env->uni_arr[i].pos, env, move_dir, move_speed);
}

void			move_camera(t_env *env, t_dvec3 *move_dir, double move_speed)
{
	vec3_move(&env->cam.pos, env, move_dir, move_speed);
}