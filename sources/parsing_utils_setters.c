/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_setters.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 19:49:38 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 22:10:10 by dromanic         ###   ########.fr       */
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
		(t_lght){(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
				 (fabs(v[3]) > 1) ? 1 : fabs(v[3]), type,
				 vec3_clamp_col_cpy((t_dvec3){v[7], v[8], v[9], 0})};
	(*id_lgh)++;
}

static void		add_cap_bottom(t_uni *dest_child, t_uni *src_parent)
{
	t_dvec3		cap_view;

	vec3_mul_double(&cap_view, &src_parent->dir, -1);
	vec3_normalize(&cap_view, &cap_view);
	*dest_child = (t_uni){
		.pos = src_parent->pos,
		.radius = src_parent->radius,
		.dir = cap_view,
		.color = src_parent->color,
		.specular = src_parent->specular,
		.get_intersect = g_intersect_catalog(DISK),
		.get_normal = g_normal_catalog(DISK),
		.reflective_coef = src_parent->reflective_coef,
		.refractive_coef = src_parent->refractive_coef,
		.is_selected = src_parent->is_selected,
	};
	set_backup_val(dest_child);
}

static void		add_cap_top(t_uni *dest_child, t_uni *src_parent)
{
	t_dvec3		cap_top_pos;

	vec3_mul_double(&cap_top_pos, &src_parent->dir, src_parent->height);
	vec3_add_vec3(&cap_top_pos, &src_parent->pos, &cap_top_pos);
	*dest_child = (t_uni){
		.pos = cap_top_pos,
		.radius = src_parent->radius,
		.dir = src_parent->dir,
		.color = src_parent->color,
		.specular = src_parent->specular,
		.get_intersect = g_intersect_catalog(DISK),
		.get_normal = g_normal_catalog(DISK),
		.reflective_coef = src_parent->reflective_coef,
		.refractive_coef = src_parent->refractive_coef,
		.is_selected = src_parent->is_selected,
	};
	set_backup_val(dest_child);
}

static void		add_cap(t_uni *uni_arr, const double *v,
					size_t *id_uni, size_t type)
{
	t_uni		*src_parent;

	if (type == CYLINDER)
	{
		src_parent = &uni_arr[*id_uni];
		(*id_uni)++;
		add_cap_bottom(&(uni_arr[*id_uni]), src_parent);
		(*id_uni)++;
		add_cap_top(&(uni_arr[*id_uni]), src_parent);
	}
	if (type == CONE)
	{
		src_parent = &uni_arr[*id_uni];
		(*id_uni)++;
		add_cap_top(&(uni_arr[*id_uni]), src_parent);
	}
	(*id_uni)++;
}

static void		set_uni(t_uni *uni_arr, const double *v,
					size_t *id_uni, size_t type)
{
	uni_arr[*id_uni] =
		(t_uni){(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
			fabs(v[3]), (t_dvec3){v[4], v[5], v[6], 0.0},
			fabs(v[7]),
			vec3_clamp_col_cpy((t_dvec3){v[8], v[9], v[10], 0}), v[11],
			g_intersect_catalog(type), g_normal_catalog(type), v[12], v[13],
			(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
			fabs(v[3]),
			(t_dvec3){v[4], v[5], v[6], 0.0}, false};
	vec3_normalize(&uni_arr[*id_uni].dir, &uni_arr[*id_uni].dir);

	if ((uni_arr[*id_uni].dir.x == 0)
	&& (uni_arr[*id_uni].dir.y == 0)
	&& (uni_arr[*id_uni].dir.z == 0))
		uni_arr[*id_uni].dir = (t_dvec3){0, 1, 0};
	add_cap(uni_arr, v, id_uni, type);
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