/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 13:11:51 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

t_uni			*intersect_obj(t_env *env, t_cam *cam, double *dist)
{
	const size_t	len = env->uni_arr_len;
	size_t			i;
	t_uni			*cur_obj;
	t_dvec3			touch;

	cur_obj = NULL;
	i = -1;
	while (++i < len)
	{
		if (env->uni_arr[i].type == SPHERE)
			intersect_sphere(&cam->pos, &env->uni_arr[i], cam->dir, &touch);
		else if (env->uni_arr[i].type == PLANE)
			intersect_plane(&cam->pos, &env->uni_arr[i], cam->dir, &touch);
		else if (env->uni_arr[i].type == CYLINDER)
			intersect_cylinder(&cam->pos, &env->uni_arr[i], cam->dir, &touch);
		else if (env->uni_arr[i].type == CONE)
			intersect_cone(&cam->pos, &env->uni_arr[i], cam->dir, &touch);
		if (touch.x < *dist && cam->t_min < touch.x && touch.x < cam->t_max
			&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.x;
		if (touch.y < *dist && cam->t_min < touch.y && touch.y < cam->t_max
			&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.y;
	}
	return (cur_obj);
}

const t_uni		*is_shadow_ray(t_env *env, t_dvec3 *ray_pos,
							t_dvec3 direction, t_dvec limits)
{
	const t_uni		*objects = env->uni_arr;
	const size_t	len = env->uni_arr_len;
	size_t			i;
	t_dvec3			touch;

	i = -1;
	while (++i < len)
	{
		if (objects[i].type == SPHERE)
			intersect_sphere(ray_pos, &objects[i], direction, &touch);
		else if (objects[i].type == PLANE)
			intersect_plane(ray_pos, &objects[i], direction,  &touch);
		else if (objects[i].type == CYLINDER)
			intersect_cylinder(ray_pos, &objects[i], direction,  &touch);
		else if (objects[i].type == CONE)
			intersect_cone(ray_pos, &objects[i], direction,  &touch);
		if ((touch.x < MAXFLOAT
			&& limits.x < touch.x
			&& touch.x < limits.y)
		|| (touch.y < MAXFLOAT
			&& limits.x < touch.y
			&& touch.y < limits.y))
			return (&objects[i]);
	}
	return (NULL);
}

t_dvec3				get_normal(t_cam *cam, t_uni *obj,
								t_dvec3 point, double dist)
{
	t_dvec3		obj_normal;
	double		k;
	t_dvec3		v;

	obj_normal = (t_dvec3){0};
	if (obj->type == SPHERE)
		obj_normal = vec3_sub_vec3(point, obj->pos);
	if (obj->type == PLANE)
		obj_normal = obj->dir;
	if (obj->type == CYLINDER)
	{
		v = obj->dir;
		obj_normal = (vec3_sub_vec3(vec3_sub_vec3(point, obj->pos),
			vec3_mul_double(v, vec3_dot_vec3(cam->dir, v)
			* dist + vec3_dot_vec3(vec3_sub_vec3(cam->pos, obj->pos), v))));
	}
	if (obj->type == CONE)
	{
		v = obj->dir;
		k = ((t_cone *)obj)->angle * M_PI / 360.0;
		obj_normal = vec3_sub_vec3(vec3_sub_vec3(point, obj->pos),
			vec3_mul_double(double_mul_vec3((1 + k * k), v),
				vec3_dot_vec3(cam->dir, v) * dist
					+ vec3_dot_vec3(vec3_sub_vec3(cam->pos, obj->pos), v)));
	}
	return (vec3_normalize(obj_normal));
}

void				send_ray(t_env *env, t_cam *cam, t_dvec3 *color)
{
	double			dist;
	t_uni			*obj;
	t_lght_comp		l;

	dist = MAXFLOAT;
	ft_bzero(&l, sizeof(t_lght_comp));
	if (!(obj = intersect_obj(env, &env->cam, &dist)))
		*color = env->bg_color;
	else
	{
		l.touch_point =
			vec3_add_vec3(cam->pos, double_mul_vec3(dist, cam->dir));
		l.view = double_mul_vec3(-1, cam->dir);
		l.obj_normal = get_normal(&env->cam, obj, l.touch_point, dist);
		l.touch_point = vec3_add_vec3(l.touch_point,
							vec3_mul_double(l.obj_normal, env->epsilon));
		*color = get_light(env, &l, obj);
	}
}