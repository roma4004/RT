/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_setters.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 19:49:38 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 21:22:05 by dromanic         ###   ########.fr       */
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
					uint64_t *id_lgh, uint64_t type)
{
	light_arr[*id_lgh] =
		(t_lght){
			(t_dvec3){
				.x = v[0],
				.y = v[1],
				.z = v[2], 0.0},
			v[3],
			type,
			(t_dvec3){
				.x = v[4],
				.y = v[5],
				.z = v[6], 0.0}
	};
	ft_clamp_in_range(&light_arr[*id_lgh].intensity,
		light_arr[*id_lgh].intensity, 0.0, 1.0);
	ft_clamp_in_range_vec(&light_arr[*id_lgh].col, 0.0, 255.0);
	vec3_div_double(&light_arr[*id_lgh].col,
		&light_arr[*id_lgh].col, 255.0);
	vec3_mul_double(&light_arr[*id_lgh].col,
		&light_arr[*id_lgh].col, light_arr[*id_lgh].intensity);
	(*id_lgh)++;
}

static void		set_arr(t_uni *arr, const double *v,
					uint64_t *id, uint64_t type)
{
	arr[*id] =
		(t_uni){(t_dvec3){.x = v[0], .y = v[1], .z = v[2], 0.0},
			v[3], (t_dvec3){v[4], v[5], v[6], 0.0}, fabs(v[7]),
			vec3_clamp_col_cpy((t_dvec3){v[8], v[9], v[10], 0.0}), v[11],
			g_intersect_catalog(type), g_normal_catalog(type), v[12], v[13],
			v[14],
			.is_selected = false};
	vec3_normalize(&arr[*id].dir, &arr[*id].dir);
	set_backup_val(&arr[*id]);
	if ((arr[*id].dir.x == 0.0)
	&& (arr[*id].dir.y == 0.0)
	&& (arr[*id].dir.z == 0.0))
		arr[*id].dir = (t_dvec3){0.0, 1.0, 0.0, 0.0};
	ft_clamp_in_range(&arr[*id].reflective_coef,
		arr[*id].reflective_coef, 0.0, 1.0);
	ft_clamp_in_range(&arr[*id].refractive_coef,
		arr[*id].refractive_coef, 0.0, 1.0);
	if (type == CONE || type == CONENEG)
		arr[*id].cone_angle_cache =
			arr[*id].radius / arr[*id].height;
	add_caps(arr, id, type);
	(*id)++;
}

void			set_obj_value(t_env *env, double *v, uint64_t type)
{
	static uint64_t		id_uni = 0;
	static uint64_t		id_neg = 0;
	static uint64_t		id_lgh = 0;

	if (type == UINT64_MAX)
		return ;
	if (type == CAM)
		set_cam(&env->cam, v);
	else if (type == SCRN)
	{
		ft_clamp_in_range(&v[0], v[0], 200.0, 2000.0);
		ft_clamp_in_range(&v[1], v[1], 200.0, 2000.0);
		env->buff_width = (Uint32)v[0];
		env->buff_height = (Uint32)v[1];
	}
	else if (type < 3)
		set_light(env->light_arr, v, &id_lgh, type);
	else if (type >= OBJ_TYPE_MAX && type < SCRN)
		set_arr(env->neg_arr, v, &id_neg, type);
	else if (type < OBJ_TYPE_MAX)
		set_arr(env->uni_arr, v, &id_uni, type);
}
