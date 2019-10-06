/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/06 20:25:48 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static t_uni		*intersect_obj(t_env *env, t_ray *ray, double *dist)
{
	const size_t	len = env->uni_arr_len;
	size_t			i;
	t_uni			*cur_obj;
	t_dvec3			touch;

	cur_obj = NULL;
	i = UINT64_MAX;
	while (++i < len)
	{
		env->uni_arr[i].get_intersect(&env->uni_arr[i], &ray->pos,
										&ray->dir, &touch);
		if (touch.x < *dist && ray->t_min < touch.x && touch.x < ray->t_max
			&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.x;
		if (touch.y < *dist && ray->t_min < touch.y && touch.y < ray->t_max
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

t_dvec3		ReflectRay(t_dvec3 *R, t_dvec3 *N)
{
	double		normal_dot_dir;
	t_dvec3		normal_x2;
	t_dvec3		tmp;

	vec3_dot_vec3(&normal_dot_dir, N, R);
	double_mul_vec3(&normal_x2, 2, N);
	vec3_mul_double(&tmp, &normal_x2, normal_dot_dir);
	vec3_sub_vec3(&tmp, &tmp, R);
	return (tmp);
}

void				send_reflect_ray(t_env *env, t_ray *ray,
										t_dvec3 *cur_color, t_lght_comp *l)
{
	t_ray		ref_ray;
	t_dvec3		reflected_color;

	if (ray->reflect_coef > 1.0)
		ray->reflect_coef = 1.0;
	if (ray->reflect_coef < 0.0)
		ray->reflect_coef = 0.0;
	if (ray->dept_limit <= 0 || ray->reflect_coef <= 0)
		return ;
	else
	{
		ref_ray = (t_ray) {.t_min = 0.001,
							.t_max = (double) INFINITY,
							.pos = ray->touch_point,
							.dir = ReflectRay(&l->view, &l->normal),
							.dept_limit = ray->dept_limit - 1};
		send_ray(env, &ref_ray, &reflected_color);
		vec3_mul_double(cur_color, cur_color, 1.0 - ray->reflect_coef);
		vec3_mul_double(&reflected_color, &reflected_color, ray->reflect_coef);
		vec3_add_vec3(cur_color, cur_color, &reflected_color);
	}
}

void				send_ray(t_env *env, t_ray *ray, t_dvec3 *cur_color)
{
	double			dist;
	t_dvec3			ray_len;
	t_dvec3			epsi_normal;
	t_uni			*obj;
	t_lght_comp		l;

	dist = (double)MAXFLOAT;
	ft_bzero(&l, sizeof(t_lght_comp));
	if (!(obj = intersect_obj(env, ray, &dist)))
		*cur_color = env->bg_color;
	else
	{
		double_mul_vec3(&ray_len, dist, &ray->dir);
		vec3_add_vec3(&ray->touch_point, &ray->pos, &ray_len);
		double_mul_vec3(&l.view,-1, &ray->dir);
		obj->get_normal(ray, obj, dist, &l.normal);
		vec3_mul_double(&epsi_normal, &l.normal, env->epsilon);
		vec3_add_vec3(&ray->touch_point, &ray->touch_point, &epsi_normal);
		l.touch_point = ray->touch_point;
		get_light(env, &l, obj, cur_color);
		if (obj->is_selected)
			*cur_color = (t_dvec3){0,0,0};
		ray->reflect_coef = obj->reflective_coef;
		send_reflect_ray(env, ray, cur_color, &l);
	}
}
