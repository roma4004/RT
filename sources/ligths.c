/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 17:18:09 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		point_or_directional(t_lght *light, t_dvec3 *light_vector,
					double *t_max, t_dvec3 *touch_point)
{
	if (light->type == POINT)
	{
		vec3_sub_vec3(light_vector, &light->pos, touch_point);
		*t_max = 1.0;
	}
	else
	{
		*light_vector = light->pos;
		*t_max = (double)MAXFLOAT;
	}
}

static void		set_diffuse_val(t_lght_comp *l, const t_dvec3 *normal)
{
	double			normal_dot_light_dir;
	double			light_dir_length;
	double			normal_length;
	double			new_defuse_val;

	vec3_dot_vec3(&normal_dot_light_dir, normal, &l->dir);
	if (normal_dot_light_dir > 0.0)
	{
		vec3_length(&normal_length, normal);
		vec3_length(&light_dir_length, &l->dir);
		new_defuse_val = l->cur->intensity * normal_dot_light_dir
					/ (normal_length * light_dir_length);
		if (new_defuse_val > 0.0)
			l->defuse_val += new_defuse_val;
	}
}

static void		set_specular_val(t_lght_comp *l, const t_dvec3 *normal,
					double specular)
{
	t_dvec3		vec_reflect;
	t_dvec3		tmp_vec;
	double		tmp;
	double		reflect_dot_view;
	double		view_len;

	vec3_dot_vec3(&tmp, normal, &l->dir);
	double_mul_vec3(&tmp_vec, 2.0 * tmp, normal);
	vec3_sub_vec3(&vec_reflect, &tmp_vec, &l->dir);
	vec3_dot_vec3(&reflect_dot_view, &vec_reflect, &l->view);
	if (reflect_dot_view > 0.0)
	{
		vec3_length(&tmp, &vec_reflect);
		vec3_length(&view_len, &l->view);
		if (specular > 0.0)
			l->specul_val += l->cur->intensity * pow(reflect_dot_view
				/ (tmp * view_len), specular);
	}
}

void			get_light(t_env *env, t_lght_comp *l,
					const t_uni *obj, t_dvec3 *col, t_ray *ray)
{
	size_t			i;
	t_dvec3			defuse_col;
	t_dvec3			specul_col;
	double			t_max;

	i = UINT64_MAX;
	while (++i < env->light_arr_len && (l->cur = &env->light_arr[i]))
	{
		if (l->cur->intensity <= 0.0)
			continue;
		if (l->cur->type == AMBIENT)
		{
			l->defuse_val += l->cur->intensity;
		}
		else
		{
			point_or_directional(l->cur, &l->dir, &t_max, &ray->touch_point);
			if (is_shadow_ray(env, ray, &l->dir, t_max))
				continue;
			set_diffuse_val(l, &ray->normal);
			set_specular_val(l, &ray->normal, obj->specular);
		}
	}
	t_dvec3		orig_col;
//	double_mul_vec3(&orig_col, 1.0 - l->defuse_val , &l->cur->color);
	double_mul_vec3(&defuse_col, l->defuse_val, &obj->color);
//	vec3_add_vec3_col(&defuse_col, &defuse_col, &orig_col);
//	//todo: color light
//	double		orig_col_percent;
//	double		val = 1.0 - l->defuse_val;
////
//	ft_clamp_in_range(&orig_col_percent, &val, 0.0, 1.0);
//	orig_col_percent = val;
//	double_mul_vec3(&orig_col, orig_col_percent, &obj->color);


	double_mul_vec3(&specul_col, l->specul_val, &l->cur->color);
//	vec3_add_vec3_col(&defuse_col, &defuse_col, &orig_col);
	vec3_add_vec3_col(col, &defuse_col, &specul_col);
}//сумму коеф зеркальности и прозрачности не должна быть меньше чем 0.98
