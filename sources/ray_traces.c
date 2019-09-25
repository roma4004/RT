/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/25 15:17:08 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		intersect_sphere(t_dvec3 ray_pos, t_uni *obj,
								t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	const t_dvec3		center = obj->pos;
	const double		radius = obj->radius;
	t_dvec3				oc;
	t_dvec3				tmp;
	double				discriminant;

	oc = vec3_sub_vec3(ray_pos, center);
	tmp.x = vec3_dot_vec3(ray_dir, ray_dir);
	tmp.y = vec3_to_double(double_mul_vec3(2, vec3_mul_vec3(oc, ray_dir)));
	tmp.z = vec3_dot_vec3(oc, oc) - radius * radius;

	discriminant = tmp.y * tmp.y - 4 * tmp.x * tmp.z;
	if (discriminant < 0)
		*plane_toch = (t_dvec3){MAXFLOAT, MAXFLOAT, 0};
	*plane_toch = (t_dvec3){(-tmp.y + sqrtf(discriminant)) / (2 * tmp.x),
							(-tmp.y - sqrtf(discriminant)) / (2 * tmp.x), 0};
}

static void			intersect_plane(t_dvec3 ray_pos, t_uni *plane,
								t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	t_dvec3		plane_pos;		//plane center		// (plane.pos)
	t_dvec3		normal;			//plane normal		// (plane.ray_dir)
//	t_dvec3		ray_pos;		//ray pos			// (cam.pos)
//	t_dvec3		ray_dir;		//ray directional	// (cam.ray_dir)
	double		denom;

	plane_pos = plane->pos;
	normal    = plane->dir;
	denom = vec3_dot_vec3(normal, ray_dir);
	if (denom > 1e-6 || denom < 1e-6)
	{
		*plane_toch =
			(t_dvec3){vec3_dot_vec3(vec3_sub_vec3(plane_pos, ray_pos), normal)
			/ denom, 0, 0};
	}
}

t_uni				*intersect_obj(t_env *env, double *dist)//dist need move to ray struct
{
	size_t		i;
	t_uni		*cur_obj;
	t_dvec3		touch;

	cur_obj = NULL;
	i = -1;
	while (++i < SPHERE_CNT + PLANE_CNT)
	{
		if (env->uni_arr[i].type == SPHERE)
			intersect_sphere(env->cam.pos, &env->uni_arr[i], env->cam.dir, &touch);
		else if (env->uni_arr[i].type == PLANE)
			intersect_plane(env->cam.pos, &env->uni_arr[i], env->cam.dir, &touch);
		if (touch.x < *dist //compute nearly obj
		&& env->cam.t_min < touch.x
		&& touch.x < env->cam.t_max
		&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.x;
		if (touch.y < *dist
		&& env->cam.t_min < touch.y
		&& touch.y < env->cam.t_max
		&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.y;
	}
	return (cur_obj);
}

t_uni		*is_shadow_ray(t_uni *uni_arr, t_dvec3 origin,
							t_dvec3 direction, t_dvec limits)
{
	size_t		i;
	t_dvec3		touch;

	i = -1;
	while (++i < SPHERE_CNT + PLANE_CNT)
	{
		if (uni_arr[i].type == SPHERE)
			intersect_sphere(origin, &uni_arr[i], direction, &touch);
		else if (uni_arr[i].type == PLANE)
			intersect_plane(origin, &uni_arr[i], direction,  &touch);
		if ((touch.x < MAXFLOAT
			&& limits.x < touch.x
			&& touch.x < limits.y)
		|| (touch.y < MAXFLOAT
			&& limits.x < touch.y
			&& touch.y < limits.y))
			return (&uni_arr[i]);
	}
	return (NULL);
}

t_dvec3				send_ray(t_env *env)
{
	double		dist;
	t_uni		*obj;
	t_dvec3		point;
	t_dvec3		obj_normal;
	t_dvec3		view;

	dist = MAXFLOAT;
	if (!(obj = intersect_obj(env, &dist)))
		return (env->bg_color);
//	if (obj->type == PLANE)
//		return (obj->mat.diffuse_color);
//	if (obj->type == SPHERE)
//	{
		point = vec3_add_vec3(env->cam.pos, double_mul_vec3(dist, env->cam.dir)); //ray not cam
		obj_normal = vec3_sub_vec3(point, obj->pos);
		obj_normal = double_mul_vec3(1.0f / vec3_mag(&obj_normal), obj_normal);
		view = double_mul_vec3(-1, env->cam.dir);

	if (obj->type == PLANE)
		return (get_light(env, point, &obj->dir, view, obj));

	return (get_light(env, point, &obj_normal, view, obj));
//	}
//	else
//		return (obj->mat.diffuse_color);

//	return (cur->mat.diffuse_color);
}