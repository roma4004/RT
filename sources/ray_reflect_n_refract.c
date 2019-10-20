/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_reflect_n_refract.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 15:56:23 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/20 16:01:27 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_dvec3		compute_refract_dir(double eta, double cosi,
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

static t_dvec3		refract(const t_dvec3 *dir, const t_dvec3 *normal,
						const double *obj_refract_coef)
{
	double		cosi;
	double		etai;
	double		etat;
	t_dvec3		*ref_normal;

	etai = 1.0;
	etat = *obj_refract_coef;
	ref_normal = (t_dvec3 *)normal;
	vec3_dot_vec3(&cosi, dir, normal);
	ft_clamp_in_range(&cosi, cosi, -1.0, 1.0);
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

	if (ray->dept_limit <= 0
		|| ray->reflect_coef <= 0.0)
		return ;
	else
	{
		ref_ray = (t_ray){
			.t_min = 0.001,
			.t_max = (double)INFINITY,
			.pos = ray->touch_point,
			.dept_limit = ray->dept_limit - 1,
			.dir = refract(&l->view, &ray->normal, &ray->refract_coef)};
		send_ray(env, &ref_ray, &reflected_color);
		vec3_mul_double(cur_color, cur_color, 1.0 - ray->refract_coef);
		vec3_mul_double(&reflected_color, &reflected_color, ray->refract_coef);
		vec3_add_vec3(cur_color, cur_color, &reflected_color);
	}
}

void				send_reflect_ray(t_env *env, t_ray *ray,
						t_dvec3 *cur_color, t_lght_comp *l)
{
	t_ray		ref_ray;
	t_dvec3		reflected_color;
	double		cos_i;
	t_dvec3		normal_x2;
	t_dvec3		refl_dir;

	if (ray->dept_limit <= 0 || ray->reflect_coef <= 0)
		return ;
	else
	{
		vec3_dot_vec3(&cos_i, &ray->normal, &l->view);
		double_mul_vec3(&normal_x2, 2, &ray->normal);
		vec3_mul_double(&refl_dir, &normal_x2, cos_i);
		vec3_sub_vec3(&refl_dir, &refl_dir, &l->view);
		ref_ray = (t_ray){.t_min = 0.001, .t_max = (double)INFINITY,
			.pos = ray->touch_point, .dir = refl_dir,
			.dept_limit = ray->dept_limit - 1};
		send_ray(env, &ref_ray, &reflected_color);
		vec3_mul_double(cur_color, cur_color, 1.0 - ray->reflect_coef);
		vec3_mul_double(&reflected_color, &reflected_color, ray->reflect_coef);
		vec3_add_vec3(cur_color, cur_color, &reflected_color);
	}
}
