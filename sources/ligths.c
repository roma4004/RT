/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/18 12:29:24 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
float		get_diffuse_reflection(float light_intensity, float normal_dot_light, float normal_magnitude, t_fvec3 *light_vector)
{
	float		intensity;

	if (normal_dot_light > 0.0f && light_intensity > 0.0f)
	{
		intensity = light_intensity * normal_dot_light
					/ (normal_magnitude * vec3_mag(light_vector));
		if (intensity > 0.0f)
			return (intensity);
	}
	return (0);
}

void				point_or_directional(t_lght *light, t_fvec3 *light_vector,
										float *t_max, t_fvec3 *point)
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


//float				get_diffuse_reflection(t_lght *light, t_fvec3 *light_vector,
//											int specular, t_fvec3 *point)
//{
//	float			reflect_dot_view;
//
//	if (specular != -1)// Specular reflection.
//	{
//		t_fvec3		vec_reflect;
//		vec_reflect = vec3_sub_vec3(float_mul_vec3(2.0 * vec3_dot_vec3(normal, light_vector), normal), light_vector);
//		if ((reflect_dot_view = vec3_dot_vec3(vec_reflect, view)) > 0)
//			intensity += light->intensity * pow(reflect_dot_view / (vec3_mag(&vec_reflect) * length.y), specular);
//	}
//}

t_fvec3				reflect(t_fvec3 normal, t_fvec3 *light_vector, t_fvec3 *vec_reflect)
{
	const float		light_normal = vec3_dot_vec3(normal, *light_vector);

//	*vec_reflect = vec3_sub_vec3(float_mul_vec3(2.0f * light_normal, normal),
//						*light_vector);
	*vec_reflect = vec3_sub_vec3(
		float_mul_vec3(light_normal, float_mul_vec3(2.0f, normal)),
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
t_fvec3				float_mul_vec3_col(double first, t_fvec3 second)
{
	return ((t_fvec3){double2byte_clamp(first * second.x),
					double2byte_clamp(first * second.y),
					double2byte_clamp(first * second.z)});
}

t_fvec3				vec3_add_vec3_col(t_fvec3 first, t_fvec3 second)
{
	return ((t_fvec3){double2byte_clamp(first.x + second.x),
					double2byte_clamp(first.y + second.y),
					double2byte_clamp(first.z + second.z)});
}


float				get_specular_reflection(float light_intensity, float specular_exponent, t_fvec3 normal, t_fvec3 view, t_fvec3 *dir)
{
	t_fvec3			vec_reflect;
	float			reflect_dot_view;

	if (light_intensity > 0.0f
	&& ((reflect_dot_view = vec3_dot_vec3(reflect(normal, dir, &vec_reflect), view)) > 0))
	{
		return (
			//		light_intensity * powf(reflect_dot_view / (vec3_length(vec_reflect) * length.y), 10.);
			light_intensity
			* powf(reflect_dot_view /
					(vec3_length(vec_reflect) * vec3_length(view)),
				specular_exponent));
	}
	return (0);
}

t_fvec3			get_light(t_env *env, t_fvec3 point,
							t_fvec3 normal, t_fvec3 view,
							t_sphr *obj)
{
	float			intensity_defuse;
	t_lght			*light;
	const t_fvec	length = (t_fvec){vec3_length(normal), vec3_length(view)};// Should be 1.0, but just in case...
	t_fvec3			dir;
	float			t_max;
	float	intensity_spec = 0;
	intensity_defuse = 0;
	unsigned i = -1;
	while (++i < LIGHTS_CNT && (light = &env->light_arr[i]))
	{
		if (light->type == AMBIENT)
			intensity_defuse += light->intensity;
		else
		{
			point_or_directional(light, &dir, &t_max, &point);
			if (is_shadow_ray(env, point, dir, (t_fvec){env->epsilon, t_max}))
				continue;
			intensity_defuse += get_diffuse_reflection(light->intensity, vec3_dot_vec3(normal, dir), length.x, &dir);
			while (intensity_defuse > 1.0f)
				intensity_defuse -= 0.01f;
			if (obj->mat.specular_exponent > 0.0
			&& (intensity_spec = get_specular_reflection(light->intensity, obj->mat.specular_exponent, normal, view, &dir))
			)
				while (intensity_defuse > 0.0f
				&& intensity_defuse + intensity_spec > 1.0f)
					intensity_defuse -= 0.01f;
		}
	}
	return (vec3_add_vec3(
//				vec3_mul_vec3(float_mul_vec3(intensity_defuse, float_mul_vec3(obj->mat.albedo, (t_fvec3){255, 255, 255})), obj->mat.diffuse_color),
//				float_mul_vec3(intensity_defuse * obj->mat.albedo, vec3_add_vec3(float_mul_vec3(obj->mat.albedo, vec3_sub_vec3((t_fvec3){255, 255, 255},obj->mat.diffuse_color)), obj->mat.diffuse_color)),
				float_mul_vec3(intensity_defuse, obj->mat.diffuse_color),
				float_mul_vec3(intensity_spec, (t_fvec3){255, 255, 255})
				)
			);
//	else
//		return (float_mul_vec3(intensity_defuse, obj->mat.diffuse_color));
//	t_fvec3 light_vec = float_mul_vec3(intensity_defuse, obj->mat.diffuse_color);
//
//t_fvec3		left = float_mul_vec3_col(obj->mat.albedo.x, float_mul_vec3_col(intensity_defuse, obj->mat.diffuse_color));
//t_fvec3		right = float_mul_vec3_col(obj->mat.albedo.y, float_mul_vec3_col(intensity_defuse,(t_fvec3){255,255,255}));

//	return (vec3_add_vec3(left,right)
//
//			);
}