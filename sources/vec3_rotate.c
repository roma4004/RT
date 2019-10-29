/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_rotate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 14:59:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 17:30:17 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	rotate_x(t_dvec3 *destination,
			const t_dvec3 *restrict pt,
			double angle)
{
	*destination = (t_dvec3){
		.x = pt->x * cos(angle) + pt->z * sin(angle),
		.y = pt->y,
		.z = -pt->x * sin(angle) + pt->z * cos(angle)};
}

void	rotate_y(t_dvec3 *destination,
			const t_dvec3 *restrict pt,
			double angle)
{
	*destination = (t_dvec3){
		.x = pt->x,
		.y = pt->y * cos(angle) + pt->z * sin(angle),
		.z = -pt->y * sin(angle) + pt->z * cos(angle)};
}

void	rotate_z(t_dvec3 *destination,
			const t_dvec3 *restrict pt,
			double angle)
{
	*destination = (t_dvec3){
		.x = pt->x * cos(angle) - pt->y * sin(angle),
		.y = pt->x * sin(angle) + pt->y * cos(angle),
		.z = pt->z};
}

_Bool	rotate_vec(t_dvec3 *vec, const t_dvec3 *rotate_angle)
{
	size_t		status;

	status = 0;
	if (rotate_angle->x != 0.0 && ++status)
		rotate_x(vec, vec, rotate_angle->x * M_PI / 180.0);
	if (rotate_angle->y != 0.0 && ++status)
		rotate_y(vec, vec, rotate_angle->y * M_PI / 180.0);
	if (rotate_angle->z != 0.0 && ++status)
		rotate_z(vec, vec, rotate_angle->z * M_PI / 180.0);
	return (status);
}

void	rotate_objects(t_env *env, t_dvec3 rot)
{
	size_t		i;
	_Bool		rot_stat;

	i = UINT64_MAX;
	while (++i < env->uni_arr_len)
	{
		if (env->uni_arr[i].is_selected)
		{
			rot_stat = rotate_vec(&env->uni_arr[i].dir, &rot);
			if (rot_stat)
			{
				if (env->uni_arr[i].get_intersect == get_intersect_cylinder)
				{
					calc_top_cap(&env->uni_arr[i + 2], &env->uni_arr[i]);
					calc_bot_cap(&env->uni_arr[i + 1], &env->uni_arr[i]);
				}
				if (env->uni_arr[i].get_intersect == get_intersect_cone)
					calc_top_cap(&env->uni_arr[i + 1], &env->uni_arr[i]);
			}
		}
	}
}
