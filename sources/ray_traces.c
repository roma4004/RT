/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/26 22:13:37 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool			is_in_range(double val, double min, double max)
{
	if (val > min && val < max)
		return (true);
	return (false);
}

_Bool			is_in_negative_list(t_uni *obj, t_uni *neg_arr,
					size_t neg_arr_len)
{
	size_t			i;

	i = UINT64_MAX;
	while (++i < neg_arr_len)
		if (&neg_arr[i] == obj)
			return (true);
	return (false);
}

void			negative_intersect(t_list *lst, t_dvec3 *touch,
					double *dist, t_uni **obj)
{
	t_touch			*touch_neg;

	while (lst)
	{
		touch_neg = (t_touch *)lst->content;
		if ((is_in_range(touch->x, touch_neg->t2, touch_neg->t1))
		&& (is_in_range(touch->y, touch_neg->t2, touch_neg->t1))
//		&&	is_in_range(*dist, touch_neg->t2, touch_neg->t1)
		)

		{
			*dist = (double)MAXFLOAT;
			*obj = NULL;
		}
		else if (is_in_range(touch->y, touch_neg->t2, touch_neg->t1)
			)
		{
			*dist = touch_neg->t1;
			touch_neg->obj->color = (*obj)->color;
			*obj = touch_neg->obj;
		}
//		else if (is_in_range(touch->x, touch_neg->t2, touch_neg->t1)
//			&& touch_neg->t2 < *dist)
//		{
//			*dist = touch_neg->t2;
//			touch_neg->obj->color = (*obj)->color;
//			*obj = touch_neg->obj;
//		}
		lst = lst->next;
	}
}

void			shadow_negative_intersect(t_list *lst, t_dvec3 *touch,
					t_uni **obj)
{
	t_touch			*touch_neg;

	while (lst)
	{
		touch_neg = (t_touch *)lst->content;
		if ((is_in_range(touch->x, touch_neg->t2, touch_neg->t1))
		&& (is_in_range(touch->y, touch_neg->t2, touch_neg->t1)))
		{
			*obj = NULL;
		}
//		else if (is_in_range(touch->x, touch_neg->t2, touch_neg->t1))
//		{
//			*closest_t = touch_neg->t2;
//			touch_neg->obj->color = (*obj)->color;
//			*obj = touch_neg->obj;
//		}
		else if (is_in_range(touch->y, touch_neg->t2, touch_neg->t1))
		{
			touch_neg->obj->color = (*obj)->color;
			*obj = touch_neg->obj;
		}
		lst = lst->next;
	}
}

static void		get_negative_touch(t_list **lst, const t_env *env, t_ray *ray)
{
	t_list		*lst_neg;
	size_t		i;
	t_dvec3		touch;

	lst_neg = NULL;
	i = UINT64_MAX;
	while (++i < env->neg_arr_len)
	{
		env->neg_arr[i].get_intersect(&touch, &env->neg_arr[i], ray);
		t_touch touch_tmp;
		touch_tmp = (t_touch){&env->neg_arr[i], touch.x, touch.y};
		ft_lstappend(&lst_neg, &touch_tmp, sizeof(t_touch));
	}
	*lst = lst_neg;
}

t_uni			*intersect_obj(double *dist, const t_env *env, t_ray *ray)
{
	t_list			*lst_neg;
	t_uni			*obj_pos;
	size_t			i;
	t_dvec3			touch;

	get_negative_touch(&lst_neg, env, ray);
	obj_pos = NULL;
	i = UINT64_MAX;
	while (++i < env->uni_arr_len)
	{
		env->uni_arr[i].get_intersect(&touch, &env->uni_arr[i], ray);
		if (touch.x < *dist && ray->t_min < touch.x && touch.x < ray->t_max)
		{
			*dist = touch.x;
			obj_pos = &env->uni_arr[i];
		}
		if (touch.y < *dist && ray->t_min < touch.y && touch.y < ray->t_max)
		{
			*dist = touch.y;
			obj_pos = &env->uni_arr[i];
		}
		if (obj_pos)
			negative_intersect(lst_neg, &touch, dist, &obj_pos);
	}
	return (obj_pos);
}

const t_uni		*is_shadow_ray(const t_env *env, t_ray *ray,
					const t_dvec3 *shadow_dir, double t_max)
{//todo: add refractive ray
	t_uni			*obj;
	t_ray			shadow_ray;
	double			dist;

	dist = (double)MAXFLOAT;
	shadow_ray = (t_ray){
		.t_min = ray->t_min,
		.t_max = t_max,
		.pos = ray->touch_point,
		.dir = *shadow_dir};
	if ((obj = intersect_obj(&dist, env, &shadow_ray)))
		return (obj);
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
	if (!(obj = intersect_obj(&l.dist, env, ray)))
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
