/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/15 18:34:20 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void			set_diffuse_reflection(double *destination,
											t_lght_comp *light,
											t_dvec3 *normal)
{
	const double	light_intensity = light->cur->intensity;
	double			normal_dot_light_dir;
	double			light_dir_length;
	double			normal_length;
	double			intensity;

	vec3_dot_vec3(&normal_dot_light_dir, normal, &light->dir);
	if (normal_dot_light_dir > 0.0)
	{
		vec3_length(&normal_length, normal);
		vec3_length(&light_dir_length, &light->dir);
		intensity = light_intensity * normal_dot_light_dir
					/ (normal_length * light_dir_length);
		if (intensity > 0.0)
			*destination += intensity;
	}
}

static void			point_or_directional(t_lght *light, t_dvec3 *light_vector,
											double *t_max, t_dvec3 *point)
{
	if (light->type == POINT)
	{
		vec3_sub_vec3(light_vector, &light->pos, point);
		*t_max = 1.0;
	}
	else
	{
		*light_vector = light->pos;
		*t_max = (double)MAXFLOAT;
	}
}

static void			set_specular_reflection(double *destination, t_lght_comp *l,
											double specular)
{
	double		light_dot;
	double		light_normal;
	t_dvec3		normal;
	t_dvec3		vec_reflect;
	double		reflect_dot_view;

	vec3_dot_vec3(&light_dot, &l->normal, &l->dir);
	light_normal = 2.0 * light_dot;
	double_mul_vec3(&normal, light_normal, &l->normal);
	vec3_sub_vec3(&vec_reflect, &normal, &l->dir);
	vec3_dot_vec3(&reflect_dot_view, &vec_reflect, &l->view);
	if (reflect_dot_view > 0.0)
	{
		vec3_length(&l->vec_reflect_len, &vec_reflect);
		vec3_length(&l->view_len, &l->view);
		if (specular > 0.0)
			*destination += l->cur->intensity * pow(reflect_dot_view
			/ (l->vec_reflect_len * l->view_len), specular);
	}
}

void				get_light(t_env *env, t_lght_comp *l,
								t_uni *obj, t_dvec3 *col)
{
	size_t			i;
	t_dvec3			defuse_col;
	t_dvec3			specul_col;

	i = UINT64_MAX;
//	if (env->light_arr_len < 1)
//		return ;
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
			point_or_directional(l->cur, &l->dir, &l->t_max, &l->touch_point);
			if (is_shadow_ray(env, &l->touch_point, &l->dir, l->t_max))
				continue;
			set_diffuse_reflection(&l->defuse_val, l, &l->normal);
			set_specular_reflection(&l->specul_val, l, obj->specular);
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
}
