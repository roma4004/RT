/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:43:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 19:49:32 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				discriminant_comput(t_dvec3 *tmp, t_dvec3 *plane_toch)
{
	const double	discriminant = tmp->y * tmp->y - 4 * tmp->x * tmp->z;

	if (discriminant < 0.0)
		*plane_toch = (t_dvec3){MAXFLOAT, MAXFLOAT, 0.0};
	*plane_toch = (t_dvec3){(-tmp->y + sqrtf(discriminant)) / (2.0 * tmp->x),
						(-tmp->y - sqrtf(discriminant)) / (2.0 * tmp->x), 0.0};
}

static t_dvec3		vec3_clamp_col_cpy(t_dvec3 first)
{
	return ((t_dvec3){double_clamp(first.x),
						double_clamp(first.y),
						double_clamp(first.z)});
}

void				set_value(t_env *env, const double *v, size_t type)
{
	static size_t	id_uni = 0;
	static size_t	id_lgh = 0;

	if (type == UINT64_MAX)
		return ;
	if (type == 7)
	{
		env->cam.pos = (t_dvec3){v[0], v[1], v[2]};
		env->cam.rotate_angle = (t_dvec3){v[4], v[5], v[6]};
	}
	else if (type < 3)
	{
		env->light_arr[id_lgh++] =
			(t_lght){type, (t_dvec3){v[0], v[1], v[2]},
			(fabs(v[3]) > 1) ? 1 : fabs(v[3])};
	}
	else
	{
		env->uni_arr[id_uni] =
			(t_uni){type, (t_dvec3){v[0], v[1], v[2]}, fabs(v[3]),
					(t_dvec3){v[4], v[5], v[6]},
					vec3_clamp_col_cpy((t_dvec3){v[7], v[8], v[9]}), v[10]};
		env->uni_arr[id_uni].dir = vec3_normalize(env->uni_arr[id_uni].dir);
		id_uni++;
	}
}

t_dvec3				double_mul_vec3_col(double first, t_dvec3 second)
{
	return ((t_dvec3){double_clamp(first * second.x),
						double_clamp(first * second.y),
						double_clamp(first * second.z)});
}

t_dvec3				vec3_add_vec3_col(t_dvec3 first, t_dvec3 second)
{
	return ((t_dvec3){double_clamp(first.x + second.x),
						double_clamp(first.y + second.y),
						double_clamp(first.z + second.z)});
}
