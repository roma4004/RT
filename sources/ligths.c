/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/24 15:33:41 by dromanic         ###   ########.fr       */
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
t_dvec3				double_mul_vec3_col(double first, t_dvec3 second)
{
	return ((t_dvec3){double2byte_clamp(first * second.x),
					double2byte_clamp(first * second.y),
					double2byte_clamp(first * second.z)});
}

t_dvec3				vec3_add_vec3_col(t_dvec3 first, t_dvec3 second)
{
	return ((t_dvec3){double2byte_clamp(first.x + second.x),
					double2byte_clamp(first.y + second.y),
					double2byte_clamp(first.z + second.z)});
}


static double		get_specular_reflection(double light_intensity, double specular_exponent,
											t_dvec3 *normal, t_dvec3 view, t_dvec3 *dir)
{
	t_dvec3			vec_reflect;
	double			reflect_dot_view;
	double			intensity_spec;

	if (light_intensity > 0.0
	&& ((reflect_dot_view = vec3_dot_vec3(reflect(*normal, dir, &vec_reflect), view)) > 0.0))
	{
		intensity_spec =
		//		light_intensity * powf(reflect_dot_view / (vec3_length(vec_reflect) * length.y), 10.);
		light_intensity
		* powf(reflect_dot_view /
				(vec3_length(vec_reflect) * vec3_length(view)),
			specular_exponent);
		if (intensity_spec > 1.0)
			intensity_spec = 1.0;
		return (intensity_spec);
	}
	return (0.0);
}

t_dvec3			get_light(t_env *env, t_dvec3 point,
							t_dvec3 *normal, t_dvec3 view,
							t_uni *obj)
{
	t_lght_comp		l;
	unsigned		i = -1;

	ft_bzero(&l, sizeof(l));
	while (++i < LIGHTS_CNT && (l.cur = &env->light_arr[i]))
	{
		if (l.cur->type == AMBIENT)
			l.defuse_val += l.cur->intensity;
		else
		{
			point_or_directional(l.cur, &l.dir, &l.t_max, &point);
			if (is_shadow_ray(env, point, l.dir,
								(t_dvec){env->epsilon, l.t_max}))
				continue;
			set_diffuse_reflection(&l, normal);
			if (obj->mat.specular_exponent > 0.0
			&& (l.specul_val = get_specular_reflection(l.cur->intensity,
						obj->mat.specular_exponent, normal, view, &l.dir)))
				if (l.defuse_val > 0.0
				&& l.defuse_val + l.specul_val > 1.0)
					l.defuse_val = 1.0 - l.specul_val;
		}
	}
	return (vec3_add_vec3(double_mul_vec3(l.defuse_val, obj->mat.diffuse_color),
		double_mul_vec3(l.specul_val, (t_dvec3){255, 255, 255})));
}