/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 14:49:14 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 14:55:21 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		cnt_obj_type(t_env *env, t_list *lst)
{
	size_t		*lights;
	size_t		*objs;
	t_list		*cur;
	size_t		type;

	lights = &env->light_arr_len;
	objs = &env->uni_arr_len;
	cur = lst;
	while (cur)
	{
		if ((type = get_type(cur->content)) != UINT64_MAX)
		{
			if (type < 3u)
				(*lights)++;
			else if (type < OBJ_TYPE_MAX)
			{
				(*objs)++;
				if (type == CYLINDER)
				{
					(*objs)++;
					(*objs)++;
				}
				if (type == CONE)
					(*objs)++;
			}
		}
		cur = cur->next;
	}
}

_Bool			init_obj_arr(t_env *env, t_list *lst)
{
	cnt_obj_type(env, lst);
	if ((env->uni_arr = (t_uni *)malloc(sizeof(t_uni) * env->uni_arr_len))
		&& (env->light_arr = (t_lght *)malloc(sizeof(t_lght) * env->light_arr_len)))
		return (true);
	else
	{
		if (env->uni_arr)
			ft_memdel((void **)&env->uni_arr);
		if (env->light_arr)
			ft_memdel((void **)&env->light_arr);
	}
	return (false);
}

void			set_value(t_env *env, const double *v, size_t type)
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
					g_intersect_catalog(type), g_normal_catalog(type), v[12], v[13], v[14],
					(t_dvec3){.x = v[0], .y = v[1], .z = v[2]},
					fabs(v[3]),
					(t_dvec3){v[4], v[5], v[6], 0.0}, false, false};
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
				.get_intersect = g_intersect_catalog(DISK),
				.get_normal = g_normal_catalog(DISK),
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
				.get_intersect = g_intersect_catalog(DISK),
				.get_normal = g_normal_catalog(DISK),
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
				.get_intersect = g_intersect_catalog(DISK),
				.get_normal = g_normal_catalog(DISK),
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