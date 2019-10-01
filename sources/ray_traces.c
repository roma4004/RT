/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 13:50:28 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static t_uni		*intersect_obj(t_env *env, t_cam *cam, double *dist)
{
	const size_t	len = env->uni_arr_len;
	size_t			i;
	t_uni			*cur_obj;
	t_dvec3			touch;

	cur_obj = NULL;
	i = UINT64_MAX;
	while (++i < len)
	{
		env->uni_arr[i].get_intersect(&env->uni_arr[i], &cam->pos,
										&cam->dir, &touch);
		if (touch.x < *dist && cam->t_min < touch.x && touch.x < cam->t_max
			&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.x;
		if (touch.y < *dist && cam->t_min < touch.y && touch.y < cam->t_max
			&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.y;
	}
	return (cur_obj);
}

const t_uni			*is_shadow_ray(t_env *env, t_dvec3 *ray_pos,
									t_dvec3 *direction, t_dvec limits)
{
	const t_uni		*objects = env->uni_arr;
	const size_t	len = env->uni_arr_len;
	size_t			i;
	t_dvec3			touch;

	i = UINT64_MAX;
	while (++i < len)
	{
		objects[i].get_intersect(&objects[i], ray_pos, direction, &touch);
		if ((touch.x < (double)MAXFLOAT
			&& limits.x < touch.x
			&& touch.x < limits.y)
		|| (touch.y < (double)MAXFLOAT
			&& limits.x < touch.y
			&& touch.y < limits.y))
			return (&objects[i]);
	}
	return (NULL);
}

void				send_ray(t_env *env, t_cam *cam, t_dvec3 *color)
{
	double			dist;
	t_uni			*obj;
	t_lght_comp		l;

	dist = (double)MAXFLOAT;
	ft_bzero(&l, sizeof(t_lght_comp));
	if (!(obj = intersect_obj(env, &env->cam, &dist)))
		*color = env->bg_color;
	else
	{
		obj->touch_point =
			vec3_add_vec3(cam->pos, double_mul_vec3(dist, cam->dir));
		l.view = double_mul_vec3(-1, cam->dir);
		obj->get_normal(&env->cam, obj, dist, &l.obj_normal);
		obj->touch_point = vec3_add_vec3(obj->touch_point,
							vec3_mul_double(l.obj_normal, env->epsilon));
		get_light(env, &l, obj, color);
	}
}
