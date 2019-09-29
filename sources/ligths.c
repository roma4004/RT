/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 13:12:28 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		set_diffuse_reflection(t_lght_comp *light, t_dvec3 *normal, double *defuse_val)
{
	const double	normal_length = vec3_length(*normal);
	const double	light_dir_length = vec3_length(light->dir);
	const double	normal_dot_light_dir = vec3_dot_vec3(*normal, light->dir);
	const double	light_intensity = light->cur->intensity;
	double			intensity;

	if (normal_dot_light_dir > 0.0f && light_intensity > 0.0f)
	{
		intensity = light_intensity * normal_dot_light_dir
					/ (normal_length * light_dir_length);
		if (intensity > 0.0f)
			*defuse_val += intensity;
		if (*defuse_val > 1.0f)
			*defuse_val = 1.0f;
	}
}

void				point_or_directional(t_lght *light, t_dvec3 *light_vector,
										double *t_max, t_dvec3 *point)
{
	if (light->type == POINT)
	{
		*light_vector = vec3_sub_vec3(light->pos, *point);
		*t_max = 1.0;
	}
	else
	{
		*light_vector = light->pos;
		*t_max = MAXFLOAT;
	}
}

static double		get_specular_reflection(double light_intensity,
											double specular,
											t_dvec3 *normal,
											t_dvec3 view, t_dvec3 *dir)
{
	t_dvec3		vec_reflect;
	double		light_dot_normal;
	double		reflect_dot_view;
	double		intensity_spec;

	if (light_intensity > 0.0)
	{
		light_dot_normal = vec3_dot_vec3(*normal, *dir);
		vec_reflect =
			vec3_sub_vec3(
				double_mul_vec3(2.0f * light_dot_normal, *normal), *dir);
		if ((reflect_dot_view = vec3_dot_vec3(vec_reflect, view)) > 0.0)
		{
			intensity_spec = light_intensity * powf(reflect_dot_view
					/ (vec3_length(vec_reflect) * vec3_length(view)), specular);
			if (intensity_spec > 1.0)
				intensity_spec = 1.0;
			return (intensity_spec);
		}
	}
	return (0.0);
}

t_dvec3			get_light(t_env *env, t_lght_comp *l, t_uni *obj)
{
	unsigned		i;
	double			defuse_val;
	double			specul_val;
	const size_t	len = env->light_arr_len;

	defuse_val = 0.0;
	specul_val = 0.0;
	i = -1;
	while (++i < len && (l->cur = &env->light_arr[i]))
	{
		if (l->cur->type == AMBIENT)
			defuse_val += l->cur->intensity;
		else
		{
			point_or_directional(l->cur, &l->dir, &l->t_max, &l->touch_point);
			if (is_shadow_ray(env, &l->touch_point, l->dir,
								(t_dvec){env->epsilon, l->t_max}))
				continue;
			set_diffuse_reflection(l, &l->obj_normal, &defuse_val);
			if (obj->specular > 0.0
			&& (specul_val += get_specular_reflection(l->cur->intensity,
					obj->specular, &l->obj_normal, l->view, &l->dir)))
				if (defuse_val > 0.0
				&& defuse_val + specul_val > 1.0)
					defuse_val = 1.0 - specul_val;
		}
	}
	return (vec3_add_vec3(double_mul_vec3(defuse_val, obj->diffuse_color),
		double_mul_vec3(specul_val, (t_dvec3){255, 255, 255})));
}