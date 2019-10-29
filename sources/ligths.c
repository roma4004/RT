/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 20:56:05 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		point_or_directional(t_dvec3 *light_vector, double *t_max,
					const t_lght *light, const t_dvec3 *touch_point)
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

static void		set_diffuse_val(t_dvec3 *defuse_intens, const t_lght_comp *l,
					const t_dvec3 *normal)
{
	double			normal_dot_light_dir;
	double			light_dir_length;
	double			normal_length;
	t_dvec3			new_defuse_val;
	t_dvec3			tmp;

	vec3_dot_vec3(&normal_dot_light_dir, normal, &l->dir);
	if (normal_dot_light_dir > 0.0)
	{
		vec3_length(&normal_length, normal);
		vec3_length(&light_dir_length, &l->dir);
		vec3_mul_double(&tmp, &l->cur->col, normal_dot_light_dir);
		vec3_div_double(&new_defuse_val, &tmp,
			normal_length * light_dir_length);
		if (new_defuse_val.x > 0.0
		|| new_defuse_val.y > 0.0
		|| new_defuse_val.z > 0.0)
			vec3_add_vec3(defuse_intens, defuse_intens, &new_defuse_val);
	}
}

static void		set_specular_val(t_dvec3 *specul_intens, t_lght_comp *l,
					const t_dvec3 *normal, double specular)
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
		{
			vec3_mul_double(&tmp_vec, &l->cur->col,
				pow(reflect_dot_view / (tmp * view_len), specular));
			vec3_add_vec3(specul_intens, specul_intens, &tmp_vec);
		}
	}
}

void			get_light(t_dvec3 *col, t_lght_comp *l, const t_env *env,
					const t_ray *ray)
{
	size_t			i;
	t_dvec3			defuse_col;
	t_dvec3			specul_col;
	double			t_max;

	i = UINT64_MAX;
	while (++i < env->light_arr_len && (l->cur = &env->light_arr[i]))
	{
		if (l->cur->col.x <= 0. && l->cur->col.y <= 0. && l->cur->col.z <= 0.)
			continue;
		else if (l->cur->type == AMBIENT)
			vec3_add_vec3_col(&l->defuse_intns, &l->defuse_intns, &l->cur->col);
		else
		{
			point_or_directional(&l->dir, &t_max, l->cur, &ray->touch_point);
			if (is_shadow_ray(env, ray, &l->dir, t_max))
				continue;
			set_diffuse_val(&l->defuse_intns, l, &ray->normal);
			set_specular_val(&l->specul_intns, l, &ray->normal,
				l->obj_specular);
		}
	}
	vec3_mul_vec3(&defuse_col, &l->defuse_intns, &l->obj_color);
	vec3_mul_vec3(&specul_col, &l->specul_intns, &l->obj_color);
	vec3_add_vec3_col(col, &defuse_col, &specul_col);
}
