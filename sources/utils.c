/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:43:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 18:20:08 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zconf.h>
#include "main.h"

void				discriminant_comput(t_dvec3 *tmp, t_dvec3 *touch)
{
	const double	discriminant = tmp->y * tmp->y - 4 * tmp->x * tmp->z;

	if (discriminant < 0.0)
		*touch = (t_dvec3){0.0};
	*touch = (t_dvec3){.x = (-tmp->y + sqrt(discriminant)) / (2.0 * tmp->x),
						.y = (-tmp->y - sqrt(discriminant)) / (2.0 * tmp->x),
						.z = 0.0};
}

static t_dvec3		vec3_clamp_col_cpy(t_dvec3 first) //todo: delete this func when implement JSON parsing
{
	return ((t_dvec3){double_clamp(first.x),
						double_clamp(first.y),
						double_clamp(first.z), 0.0});
}

void				set_value(t_env *env, const double *v, size_t type)
{
	static size_t	id_uni = 0;
	static size_t	id_lgh = 0;

	if (type == UINT64_MAX)
		return ;
	if (type == CAM)
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
		env->uni_arr[id_uni] =
			(t_uni){(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
			fabs(v[3]), (t_dvec3){v[4], v[5], v[6], 0.0}, //todo: test dir is not 0,0,0
			fabs(v[7]),
			vec3_clamp_col_cpy((t_dvec3){v[8], v[9], v[10], 0}), v[11],
			intersect_catalog(type), normal_catalog(type), v[12], v[13], v[14],
			(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
			fabs(v[3]),
			(t_dvec3){v[4], v[5], v[6], 0.0}, false};
		vec3_normalize(&env->uni_arr[id_uni].dir, &env->uni_arr[id_uni].dir);

		if ((env->uni_arr[id_uni].dir.x == 0)
		&& (env->uni_arr[id_uni].dir.y == 0)
		&& (env->uni_arr[id_uni].dir.z == 0))
			env->uni_arr[id_uni].dir = (t_dvec3){0, 1, 0};


//TODO: check reset for cylinder (wrong backup values)
		if (type == CYLINDER)
		{
			t_dvec3		cap_top_pos;
			t_dvec3		cap_view;
			t_uni		*cylinder;

			cylinder = &env->uni_arr[id_uni];
			id_uni++;
			vec3_mul_double(&cap_view, &cylinder->dir, -1);
			vec3_normalize(&cap_view, &cap_view);
			env->uni_arr[id_uni] = (t_uni) {
					.pos = cylinder->pos,
					.radius = cylinder->radius,
					.dir = cap_view,
					.color = cylinder->color,
					.specular = cylinder->specular,
					.get_intersect = intersect_catalog(DISK),
					.get_normal = normal_catalog(DISK),
					.reflective_coef = cylinder->reflective_coef,
					.refractive_coef = cylinder->refractive_coef,
					.pos_backup = cylinder->pos_backup,
					.dir_backup = cap_view,
					.is_selected = cylinder->is_selected,
			};
			id_uni++;
			vec3_mul_double(&cap_top_pos, &cylinder->dir, cylinder->height);
			vec3_add_vec3(&cap_top_pos, &cylinder->pos, &cap_top_pos);
			env->uni_arr[id_uni] = (t_uni) {
					.pos = cap_top_pos,
					.radius = cylinder->radius,
					.dir = cylinder->dir,
					.color = cylinder->color,
					.specular = cylinder->specular,
					.get_intersect = intersect_catalog(DISK),
					.get_normal = normal_catalog(DISK),
					.reflective_coef = cylinder->reflective_coef,
					.refractive_coef = cylinder->refractive_coef,
					.pos_backup = cap_top_pos,
					.dir_backup = cylinder->dir_backup,
					.is_selected = cylinder->is_selected,
			};
		}
		if (type == CONE)
		{
			t_dvec3		cap_top_pos;
			t_uni		*cone;

			cone = &env->uni_arr[id_uni];
			id_uni++;
			vec3_mul_double(&cap_top_pos, &cone->dir, cone->height);
			vec3_add_vec3(&cap_top_pos, &cone->pos, &cap_top_pos);
			env->uni_arr[id_uni] = (t_uni) {
					.pos = cap_top_pos,
					.radius = cone->radius,
					.dir = cone->dir,
					.color = cone->color,
					.specular = cone->specular,
					.get_intersect = intersect_catalog(DISK),
					.get_normal = normal_catalog(DISK),
					.reflective_coef = cone->reflective_coef,
					.refractive_coef = cone->refractive_coef,
					.pos_backup = cap_top_pos,
					.dir_backup = cone->dir_backup,
					.is_selected = cone->is_selected,
			};
		}
		id_uni++;
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

