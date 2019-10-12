/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:43:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/12 16:15:36 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zconf.h>
#include "main.h"

void				discriminant_comput(t_dvec3 *tmp, t_dvec3 *touch)
{
	const double	discriminant = tmp->y * tmp->y - 4 * tmp->x * tmp->z;

	if (discriminant < 0.0)
		*touch = (t_dvec3){.x = (double)INFINITY,
							.y = (double)INFINITY,
							.z = 0.0};
	*touch = (t_dvec3){.x = (-tmp->y + sqrt(discriminant)) / (2.0 * tmp->x),
						.y = (-tmp->y - sqrt(discriminant)) / (2.0 * tmp->x),
						.z = 0.0};
}

static t_dvec3		vec3_clamp_col_cpy(t_dvec3 first)
{
	return ((t_dvec3){double_clamp(first.x),
						double_clamp(first.y),
						double_clamp(first.z), 0.0});
}

void				set_value(t_env *env, const double *v, size_t type)
{
	static size_t	id_uni = ~0ul;
	static size_t	id_lgh = 0;

	if (type == UINT64_MAX)
		return ;
	if (type == 7)
	{
		env->cam.pos = (t_dvec3){v[0], v[1], v[2], 0.0};
		env->cam.rotate_angle = (t_dvec3){.x = v[4], .y = v[5], .z = v[6]};
//		vec3_normalize(&env->cam.parse_rotate_angle,
//						&env->cam.parse_rotate_angle);
		env->cam.pos_backup = env->cam.pos;
		env->cam.rotate_angle_backup = env->cam.rotate_angle;
	}
	else if (type < 3)
		env->light_arr[id_lgh++] =
			(t_lght){(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
			(fabs(v[3]) > 1) ? 1 : fabs(v[3]), type,
			vec3_clamp_col_cpy((t_dvec3){v[7], v[8], v[9], 0})};
	else
	{
		env->uni_arr[++id_uni] =
			(t_uni){(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
			fabs(v[3]), (t_dvec3){v[4], v[5], v[6], 0.0},//todo: test dir is not 0,0,0
			vec3_clamp_col_cpy((t_dvec3){v[7], v[8], v[9], 0}), v[10],
			intersect_catalog(type), normal_catalog(type), v[11], v[12], v[13],
			(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
			(t_dvec3){v[4], v[5], v[6], 0.0}, false};
		vec3_normalize(&env->uni_arr[id_uni].dir, &env->uni_arr[id_uni].dir);
	}
}

void				double_mul_vec3_col(t_dvec3 *destination,
										double first,
										const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = double_clamp(first * second->x),
							.y = double_clamp(first * second->y),
							.z = double_clamp(first * second->z)};
}

void				vec3_add_vec3_col(t_dvec3 *destination,
										const t_dvec3 *restrict first,
										const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = double_clamp(first->x + second->x),
							.y = double_clamp(first->y + second->y),
							.z = double_clamp(first->z + second->z)};
}

