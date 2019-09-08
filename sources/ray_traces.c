/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/08 19:33:53 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static t_fvec3		where_intersect(t_fvec3 cam, t_fvec3 direction, t_sphr *obj)
{
	const t_fvec3		center = obj->center;
	const float			radius = obj->radius;
	t_fvec3				OC;
	t_fvec3				discr;
	float				discriminant;

	OC = vec3_sub_vec3(cam, center);
	discr.x = vec3_dot_vec3(direction, direction);
	discr.y = vec3_to_float(float_mul_vec3(2, vec3_mul_vec3(OC, direction)));
	discr.z = vec3_dot_vec3(OC, OC) - radius * radius;

	discriminant = discr.y * discr.y - 4 * discr.x * discr.z;
	if (discriminant < 0)
		return (t_fvec3){MAXFLOAT, MAXFLOAT, 0};
	return((t_fvec3){(-discr.y + sqrtf(discriminant)) / (2 * discr.x),
					 (-discr.y - sqrtf(discriminant)) / (2 * discr.x), 0});
}

t_sphr				*intersect_obj(t_env *env, float *closest_t)
{
	size_t		i;
	t_sphr		*cur_obj;
	t_fvec3		t;

	cur_obj = NULL;
	i = -1;
	while (++i < SPHERE_CNT)
	{
		t = where_intersect(env->cam.pos, env->cam.dir,
				&env->sphere_arr[i]);
		if (t.x < *closest_t && env->cam.t_min < t.x && t.x < env->cam.t_max
			&& (cur_obj = &env->sphere_arr[i]))
			*closest_t = t.x;
		if (t.y < *closest_t && env->cam.t_min < t.y && t.y < env->cam.t_max
			&& (cur_obj = &env->sphere_arr[i]))
			*closest_t = t.y;
	}
	return (cur_obj);
}

t_fvec3				send_ray(t_env *env)
{
	float		closest_t;
	t_sphr		*obj;
	t_fvec3		point;
	t_fvec3		normal;

	closest_t = MAXFLOAT;
	if (!(obj = intersect_obj(env, &closest_t)))
		return ((t_fvec3){255, 255, 255});
	point = vec3_add_vec3(env->cam.pos, float_mul_vec3(closest_t, env->cam.dir));
	normal = vec3_sub_vec3(point, obj->center);
	normal = float_mul_vec3(1.0 / vec3_magnitude(&normal), normal);

	t_fvec3		view = float_mul_vec3(-1, env->cam.dir);
	float		lighting = get_light(point, normal, view, obj->specular, env->light_arr);

//	return (float_mul_vec3(lighting, obj->color));
	return (float_mul_vec3(lighting, obj->color));
}