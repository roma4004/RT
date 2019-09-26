/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/26 15:42:40 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		set_diffuse_reflection(t_lght_comp *light, t_dvec3 *normal)
{
	const double		normal_magnitude = vec3_length(*normal);
	const double		normal_dot_light = vec3_dot_vec3(*normal, light->dir);
	const double		light_intensity = light->cur->intensity;
	double				intensity;

	if (normal_dot_light > 0.0f && light_intensity > 0.0f)
	{
		intensity = light_intensity * normal_dot_light
					/ (normal_magnitude * vec3_mag(&light->dir));
		if (intensity > 0.0f)
			light->defuse_val += intensity;
		if (light->defuse_val > 1.0f)
			light->defuse_val = 1.0f;
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
		*light_vector = light->pos; //DIRECTIONAL
		*t_max = MAXFLOAT;
	}
}


//double				set_diffuse_reflection(t_lght *light, t_dvec3 *light_vector,
//											int specular, t_dvec3 *point)
//{
//	double			reflect_dot_view;
//
//	if (specular != -1)// Specular reflection.
//	{
//		t_dvec3		vec_reflect;
//		vec_reflect = vec3_sub_vec3(double_mul_vec3(2.0 * vec3_dot_vec3(normal, light_vector), normal), light_vector);
//		if ((reflect_dot_view = vec3_dot_vec3(vec_reflect, view)) > 0)
//			intensity += light->intensity * pow(reflect_dot_view / (vec3_mag(&vec_reflect) * length.y), specular);
//	}
//}

t_dvec3				reflect(t_dvec3 normal, t_dvec3 *light_vector, t_dvec3 *vec_reflect)
{
	const double		light_normal = vec3_dot_vec3(normal, *light_vector);

//	*vec_reflect = vec3_sub_vec3(double_mul_vec3(2.0f * light_normal, normal),
//						*light_vector);
	*vec_reflect = vec3_sub_vec3(
		double_mul_vec3(light_normal, double_mul_vec3(2.0f, normal)),
			*light_vector);
	return (*vec_reflect);
}
//Vec3f reflect(const Vec3f &I, const Vec3f &N) {
//return I - N*2.f*(I*N);
//}

static uint8_t  double2byte_clamp(const double x)
{
	if (x < 0.0)
		return (0);
	else if (x > 255.0)
		return (255);
	else
		return ((uint8_t)(255 * x));
}

static double		get_specular_reflection(double light_intensity,
											double specular,
											t_dvec3 *normal,
											t_dvec3 view, t_dvec3 *dir)
{
	t_dvec3			vec_reflect;
	double			reflect_dot_view;
	double			intensity_spec;

	if (light_intensity > 0.0
	&& ((reflect_dot_view =
		vec3_dot_vec3(reflect(*normal, dir, &vec_reflect), view)) > 0.0))
	{
		intensity_spec = light_intensity * powf(reflect_dot_view
					/ (vec3_length(vec_reflect) * vec3_length(view)), specular);
		if (intensity_spec > 1.0)
			intensity_spec = 1.0;
		return (intensity_spec);
	}
	return (0.0);
}

t_dvec3			get_light(t_env *env, t_lght_comp *l, t_uni *obj)
{
	unsigned		i;

	i = -1;
	while (++i < LIGHTS_CNT && (l->cur = &env->light_arr[i]))
	{
		if (l->cur->type == AMBIENT)
			l->defuse_val += l->cur->intensity;
		else
		{
			point_or_directional(l->cur, &l->dir, &l->t_max, &l->touch_point);
			if (is_shadow_ray(env->uni_arr, l->touch_point, l->dir,
								(t_dvec){env->epsilon, l->t_max}))
				continue;
			set_diffuse_reflection(l, &l->obj_normal);
			if (obj->mat.specular > 0.0
			&& (l->specul_val = get_specular_reflection(l->cur->intensity,
					obj->mat.specular, &l->obj_normal, l->view, &l->dir)))
				if (l->defuse_val > 0.0
				&& l->defuse_val + l->specul_val > 1.0)
					l->defuse_val = 1.0 - l->specul_val;
		}
	}
	return (vec3_add_vec3(double_mul_vec3(l->defuse_val, obj->mat.diffuse_col),
		double_mul_vec3(l->specul_val, (t_dvec3){255, 255, 255})));
}