/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:56:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/15 21:06:21 by vtlostiu         ###   ########.fr       */
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
		env->uni_arr[i].get_intersect(&env->uni_arr[i], &touch, ray);
		if (touch.x < *dist && ray->t_min < touch.x && touch.x < ray->t_max
			&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.x;
		if (touch.y < *dist && ray->t_min < touch.y && touch.y < ray->t_max
			&& (cur_obj = &env->uni_arr[i]))
			*dist = touch.y;
	}
	return (cur_obj);
}

const t_uni			*is_shadow_ray(t_env *env, t_dvec3 *touch_point,
									t_dvec3 *shadow_dir, double t_max, t_ray *ray)
{//todo: add refractive ray
	const t_uni		*objects = env->uni_arr;
	const size_t	len = env->uni_arr_len;
	size_t			i;
	t_dvec3			touch;
	t_ray			shadow_ray;

	shadow_ray = (t_ray){ .t_min = ray->t_min,
						.t_max = ray->t_max,
						.pos = *touch_point,
						.dir = *shadow_dir};
	i = UINT64_MAX;
	while (++i < len)
	{
		objects[i].get_intersect(&objects[i], &touch, &shadow_ray);
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

t_dvec3		ReflectRay(t_dvec3 *R, t_dvec3 *N)
{
	double		cos_i;
	t_dvec3		normal_x2;
	t_dvec3		tmp;

	vec3_dot_vec3(&cos_i, N, R);
	double_mul_vec3(&normal_x2, 2, N);
	vec3_mul_double(&tmp, &normal_x2, cos_i);
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

void		ft_swap(double *first, double *second)
{
	double		tmp;

	tmp = *first;
	*first = *second;
	*second = tmp;
}

void		ft_clamp_in_range(double *dest, const double *value,
								double min, double max)
{
	if (*value > max)
		*dest = max;
	else if (*value < min)
		*dest = min;
	else
		*dest = *value;
}

t_dvec3				compute_refract_dir(double eta, double cosi,
								const t_dvec3 *dir, const t_dvec3 *ref_normal)
{
	double		k;
	t_dvec3		tmp;
	t_dvec3		tmp2;

	k = 1.0 - eta * eta * (1.0 - cosi * cosi);
	if (k < 0.0)
		return ((t_dvec3){0});
	else
	{
		double_mul_vec3(&tmp, eta, dir);
		double_mul_vec3(&tmp2, (eta * cosi - sqrt(k)), ref_normal);
		vec3_add_vec3(&tmp, &tmp, &tmp2);
		return (tmp);
	}
}

t_dvec3				refract(const t_dvec3 *dir, const t_dvec3 *normal,
							const double *obj_refract_coef)
{
	double		cosi;
	double		etai;
	double		etat; //etai is the index of refraction of the medium the ray is in before entering the second medium
	t_dvec3		*ref_normal;

	etai = 1.0;
	etat = *obj_refract_coef;
	ref_normal = (t_dvec3 *)normal;
	vec3_dot_vec3(&cosi, dir, normal);
	ft_clamp_in_range(&cosi, &cosi, -1.0, 1.0);
	if (cosi < 0.0)
	{
		cosi = -cosi;
	}
	else
	{
		ft_swap(&etai, &etat);
		vec3_mul_double(ref_normal, normal, -1.0);
	}
	return (compute_refract_dir(etai / etat, cosi, dir, ref_normal));
}

void				send_refract_ray(t_env *env, t_ray *ray,
										t_dvec3 *cur_color, t_lght_comp *l)
{
	t_ray		ref_ray;
	t_dvec3		reflected_color;

	if (ray->reflect_coef > 1.0)
		ray->reflect_coef = 1.0;
	if (ray->reflect_coef < 0.0)
		ray->reflect_coef = 0.0;
	if (ray->dept_limit <= 0
	|| ray->reflect_coef <= 0.0)
		return ;
	else
	{
		ref_ray = (t_ray){.t_min = 0.001, .t_max = (double)INFINITY,
			.pos = ray->touch_point, .dept_limit = ray->dept_limit - 1,
			.dir = refract(&l->view, &l->normal, &ray->refract_coef)};
		send_ray(env, &ref_ray, &reflected_color);
		vec3_mul_double(cur_color, cur_color, 1.0 - ray->refract_coef);
		vec3_mul_double(&reflected_color, &reflected_color, ray->refract_coef);
		vec3_add_vec3(cur_color, cur_color, &reflected_color);
	}
}

double		ft_max(double first, double second)
{
	return (first > second ? first : second);
}


void				compute_refract_coef(t_dvec3 *tmp, double cos_i,
										double *kr)
{
	double		cos_t;
	double		Rs;
	double		Rp;
//	tmp = (t_dvec3){.x = etai, .y = etat, .z = sint};

	cos_t = sqrt(ft_max(0.f, 1 - tmp->z * tmp->z));
	cos_i = fabs(cos_i);
	Rs = ((tmp->y * cos_i) - (tmp->x * cos_t))
		/ ((tmp->y * cos_i) + (tmp->x * cos_t));
	Rp = ((tmp->x * cos_i) - (tmp->y * cos_t))
		/ ((tmp->x * cos_i) + (tmp->y * cos_t));
	*kr = (Rs * Rs + Rp * Rp) / 2;
}

void				fresnel(const t_dvec3 *dir, const t_dvec3 *normal,
							const double *obj_refract_coef, double *kr)
{
	double		cos_i;
	double		sin_t;
	double		eta_i;
	double		eta_t;
	t_dvec3		tmp;

	eta_i = 1;
	eta_t = *obj_refract_coef;
	vec3_dot_vec3(&cos_i, dir, normal);
	ft_clamp_in_range(&cos_i, &cos_i, -1, 1);
	if (cos_i > 0)
		ft_swap(&eta_i, &eta_t);
	sin_t = eta_i / eta_t * sqrt(ft_max(0.f, 1 - cos_i * cos_i));				// Compute sini using Snell's law
	if (sin_t >= 1)																// Total internal reflection
		*kr = 1;
	else
	{
		tmp = (t_dvec3){.x = eta_i, .y = eta_t, .z = sin_t};
		compute_refract_coef(&tmp, cos_i, kr);
	}// As a consequence of the conservation of energy, transmittance is given by: kt = 1 - kr;
}

t_dvec3		reflect(const t_dvec3 *I, const t_dvec3 *N)
{
	double			I_dot_N;
	t_dvec3			tmp;
	t_dvec3			res;

	vec3_dot_vec3(&I_dot_N, I, N);
	double_mul_vec3(&tmp, 2 * I_dot_N, N);
	vec3_sub_vec3(&res, I, &tmp);
	return (res);
}

void				send_selected_ray(t_env *env, t_ray *ray,
										t_uni **obj, double dist)
{
	*obj = intersect_obj(env, ray, &dist);
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
		vec3_mul_double(&epsi_normal, &l.normal, env->cam.epsilon);
		vec3_add_vec3(&ray->touch_point, &ray->touch_point, &epsi_normal);
		l.touch_point = ray->touch_point;
		get_light(env, &l, obj, cur_color, ray);
		if (obj->is_selected)
			*cur_color = (t_dvec3){0,0,0};
		ray->reflect_coef = obj->reflective_coef;
		ray->refract_coef = obj->refractive_coef;


//		if (ray->reflect_coef > 0.0 && ray->refract_coef <= 0.0)
//		{
//			send_refract_ray(env, ray, cur_color, &l);
//			return ;
//		}

//		t_dvec3 refractionColor;
//		// compute fresnel
//		double kr;
//		fresnel(&ray->dir, &l.normal, &ray->refract_coef, &kr);
//		double dir_dot_normal;
//		vec3_dot_vec3(&dir_dot_normal, &ray->dir, &l.normal);
//		bool is_outside = dir_dot_normal < 0;
//		double bias_val = 0.001;
//		t_dvec3 bias;
//		double_mul_vec3(&bias, bias_val, &l.normal);
//		// compute refraction if it is not a case of total internal reflection
//		if (kr < 1)
//		{
//			t_dvec3 refractionDirection = refract(&ray->dir, &l.normal, &ray->refract_coef);
//			vec3_normalize(&refractionDirection, &refractionDirection);
//
//			t_dvec3 refractionRayOrig;
//			if (is_outside)
//				vec3_sub_vec3(&refractionRayOrig, &ray->touch_point, &bias);
//			else
//				vec3_add_vec3(&refractionRayOrig, &ray->touch_point, &bias);
//
//
////			refractionColor = castRay(refractionRayOrig, refractionDirection, objects, lights, options, depth + 1);
//			t_ray		refr_ray;
//			refr_ray = (t_ray){
//				.t_min = 0.001,
//				.t_max = (double) INFINITY,
//				.pos = refractionRayOrig,
//				.dir = refractionDirection,
//				.dept_limit = ray->dept_limit - 1
//			};
			send_refract_ray(env, ray, cur_color, &l);
////			send_ray(env, &refr_ray, &refractionColor);
//		}

//		t_dvec3 reflectionDirection = reflect(&ray->dir, &l.normal);
//		vec3_normalize(&reflectionDirection, &reflectionDirection);
//
//		t_dvec3 reflectionRayOrig;
//		if (is_outside)
//			vec3_sub_vec3(&reflectionRayOrig, &ray->touch_point, &bias);
//		else
//			vec3_add_vec3(&reflectionRayOrig, &ray->touch_point, &bias);
//
////		t_dvec3 reflectionRayOrig = is_outside ? ray->touch_point + bias : ray->touch_point - bias;
////		t_dvec3 reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1);
//		t_ray		refr_ray;
//		refr_ray = (t_ray){
//			.t_min = 0.001,
//			.t_max = (double)INFINITY,
//			.pos = reflectionRayOrig,
//			.dir = reflectionDirection,
//			.dept_limit = ray->dept_limit + 1
//		};
		t_dvec3		reflectionColor;
//		send_ray(env, &refr_ray, &reflectionColor);
//		// mix the two




//		send_reflect_ray(env, ray, cur_color, &l);




//		double_mul_vec3_col(&reflectionColor, kr,  &reflectionColor);
//		double_mul_vec3_col(&refractionColor, (1 - kr), &refractionColor);
//		t_dvec3 res_color;
//
//		vec3_add_vec3_col(&res_color, &reflectionColor, &reflectionColor);
////		reflectionColor * kr + refractionColor * ;
//
//
//		vec3_add_vec3(cur_color, cur_color, &res_color);





//
		send_reflect_ray(env, ray, cur_color, &l);
	}
}
