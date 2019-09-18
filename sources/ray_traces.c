/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/16 19:46:23 by dromanic         ###   ########.fr       */
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

t_sphr				*intersect_obj(t_env *env, float *dist)//dist need move to ray struct
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
		if (t.x < *dist && env->cam.t_min < t.x && t.x < env->cam.t_max
		&& (cur_obj = &env->sphere_arr[i]))
			*dist = t.x;
		if (t.y < *dist && env->cam.t_min < t.y && t.y < env->cam.t_max
		&& (cur_obj = &env->sphere_arr[i]))
			*dist = t.y;
	}
	return (cur_obj);
}

t_sphr				*is_shadow_ray(t_env *env, t_fvec3 origin, t_fvec3 direction, t_fvec limits)
{
	size_t		i;
	t_fvec3		t;

	i = -1;
	while (++i < SPHERE_CNT)
	{
		t = where_intersect(origin, direction, &env->sphere_arr[i]);
		if ((t.x < MAXFLOAT && limits.x < t.x && t.x < limits.y)
		|| (t.y < MAXFLOAT && limits.x < t.y && t.y < limits.y))
			return (&env->sphere_arr[i]);
	}
	return (NULL);
}

t_fvec3				send_ray(t_env *env)
{
	float		dist;
	t_sphr		*obj;
	t_fvec3		point;
	t_fvec3		normal;
	t_fvec3		view;

	dist = MAXFLOAT;
	if (!(obj = intersect_obj(env, &dist)))
		return (env->bg_color);
	point = vec3_add_vec3(env->cam.pos, float_mul_vec3(dist, env->cam.dir)); //ray not cam
	normal = vec3_sub_vec3(point, obj->center);
	normal = float_mul_vec3(1.0 / vec3_mag(&normal), normal);
	view = float_mul_vec3(-1, env->cam.dir);

	return (get_light(env, point, normal, view, obj));
//	return (obj->mat.diffuse_color);
}