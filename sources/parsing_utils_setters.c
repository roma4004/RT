/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_setters.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 19:49:38 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/20 15:34:51 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		set_cam(t_cam *cam, const double *v)
{
	cam->pos = (t_dvec3){v[0], v[1], v[2], 0.0};
	cam->rotate_angle = (t_dvec3){.x = v[4], .y = v[5], .z = v[6]};
	cam->pos_backup = cam->pos;
	cam->rotate_angle_backup = cam->rotate_angle;
}

static void		set_light(t_lght *light_arr, const double *v,
					size_t *id_lgh, size_t type)
{
	light_arr[*id_lgh] =
		(t_lght){(t_dvec3){
			.x = v[0],
			.y = v[1],
			.z = v[2]},
			(fabs(v[3]) > 1) ? 1 : fabs(v[3]),
			type,
			vec3_clamp_col_cpy((t_dvec3){v[7], v[8], v[9], 0})};
	(*id_lgh)++;
}

static void		set_uni(t_uni *uni_arr, const double *v,
					size_t *id_uni, size_t type)
{
	uni_arr[*id_uni] =
		(t_uni){(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
			fabs(v[3]), (t_dvec3){v[4], v[5], v[6], 0.0}, fabs(v[7]),
			vec3_clamp_col_cpy((t_dvec3){v[8], v[9], v[10], 0}), v[11],
			g_intersect_catalog(type), g_normal_catalog(type), v[12], v[13],
			false};
	vec3_normalize(&uni_arr[*id_uni].dir, &uni_arr[*id_uni].dir);
	set_backup_val(&uni_arr[*id_uni]);
	if ((uni_arr[*id_uni].dir.x == 0)
	&& (uni_arr[*id_uni].dir.y == 0)
	&& (uni_arr[*id_uni].dir.z == 0))
		uni_arr[*id_uni].dir = (t_dvec3){0, 1, 0};
	ft_clamp_in_range(&uni_arr[*id_uni].reflective_coef,
		uni_arr[*id_uni].reflective_coef, 0.0, 1.0);
	ft_clamp_in_range(&uni_arr[*id_uni].refractive_coef,
		uni_arr[*id_uni].refractive_coef, 0.0, 1.0);
	add_caps(uni_arr, v, id_uni, type);
	(*id_uni)++;
}

void			set_obj_value(t_env *env, const double *v, size_t type)
{
	static size_t	id_uni = 0;
	static size_t	id_lgh = 0;

	if (type == UINT64_MAX)
		return ;
	if (type == CAM)
		set_cam(&env->cam, v);
	else if (type < 3)
		set_light(env->light_arr, v, &id_lgh, type);
	else
		set_uni(env->uni_arr, v, &id_uni, type);
}
