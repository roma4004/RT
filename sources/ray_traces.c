/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/23 18:33:12 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_uni	*intersect_obj(const t_env *env, const t_ray *ray, double *dist)
{
	const size_t	len = env->uni_arr_len;
	t_uni			*cur_obj;
	size_t			i;
	t_dvec3			touch;

	cur_obj = NULL;
	i = UINT64_MAX;
	while (++i < len)
	{
		env->uni_arr[i].get_intersect(&touch, &env->uni_arr[i], ray);
		if (touch.x < *dist && ray->t_min < touch.x && touch.x < ray->t_max
		&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.x;
		if (touch.y < *dist && ray->t_min < touch.y && touch.y < ray->t_max
		&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.y;
	}
	return (cur_obj);
}

const t_uni		*is_shadow_ray(const t_env *env, const t_ray *ray,
					const t_dvec3 *shadow_dir, double t_max)
{//todo: add refractive ray
	const t_uni		*objects = env->uni_arr;
	const size_t	len = env->uni_arr_len;
	size_t			i;
	t_dvec3			touch;
	t_ray			shadow_ray;

	shadow_ray = (t_ray){
		.t_min = ray->t_min,
		.t_max = ray->t_max,
		.pos = ray->touch_point,
		.dir = *shadow_dir};
	i = UINT64_MAX;
	while (++i < len)
	{
		objects[i].get_intersect(&touch, &objects[i], &shadow_ray);
		if ((touch.x < (double)MAXFLOAT
			&& env->cam.epsilon < touch.x
			&& touch.x < t_max)
		|| (touch.y < (double)MAXFLOAT
			&& env->cam.epsilon < touch.y
			&& touch.y < t_max))
			return (&objects[i]);
	}
	return (NULL);
}

void			send_selected_ray(t_uni **obj, const t_env *env,
					const t_ray *ray, double dist)
{
	const size_t	len = env->uni_arr_len;
	t_uni			*cur_obj;
	size_t			i;
	t_dvec3			touch;

	cur_obj = NULL;
	i = UINT64_MAX;
	while (++i < len)
	{
		env->uni_arr[i].get_intersect(&touch, &env->uni_arr[i], ray);
		if (touch.x < dist && ray->t_min < touch.x && touch.x < ray->t_max
			&& (cur_obj = &env->uni_arr[i]))
			dist = touch.x;
		if (touch.y < dist && ray->t_min < touch.y && touch.y < ray->t_max
			&& (cur_obj = &env->uni_arr[i]))
			dist = touch.y;
	}
	*obj = cur_obj;
}

static void		prepare_light(const t_env *env, t_ray *ray, t_lght_comp *l,
					t_uni *obj)
{
	t_dvec3			ray_len;
	t_dvec3			epsi_normal;

	double_mul_vec3(&ray_len, l->dist, &ray->dir);
	vec3_add_vec3(&ray->touch_point, &ray->pos, &ray_len);
	double_mul_vec3(&l->view, -1, &ray->dir);
	obj->get_normal(ray, obj, l->dist);
	vec3_mul_double(&epsi_normal, &ray->normal, env->cam.epsilon);
	vec3_add_vec3(&ray->touch_point, &ray->touch_point, &epsi_normal);
	l->obj_specular = obj->specular;
	l->obj_color = obj->color;
}

void			send_ray(t_dvec3 *cur_color, const t_env *env, t_ray *ray)
{
	t_uni			*obj;
	t_lght_comp		l;

	ft_bzero(&l, sizeof(t_lght_comp));
	l.dist = (double)MAXFLOAT;
	if (!(obj = intersect_obj(env, ray, &l.dist)))
		*cur_color = env->bg_color;
	else
	{
		prepare_light(env, ray, &l, obj);
		get_light(cur_color, &l, env, ray);
		if (obj->is_selected)
			*cur_color = (t_dvec3){0.0, 0.0, 0.0, 0.0};
		ray->reflect_coef = obj->reflective_coef;
		ray->refract_coef = obj->refractive_coef;
		send_refract_ray(cur_color, &l, env, ray);
		send_reflect_ray(cur_color, &l, env, ray);
	}
}
