/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_intersection_base.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 12:56:10 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/20 20:24:05 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	get_intersect_sphere(t_dvec3 *touch, const t_uni *sphere,
			const t_ray *ray)
{
	const double	radius = sphere->radius;
	t_dvec3			oc;
	t_dvec3			tmp;

	vec3_sub_vec3(&oc, &ray->pos, &sphere->pos);
	vec3_dot_vec3(&tmp.x, &ray->dir, &ray->dir);
	vec3_dot_vec3(&tmp.y, &ray->dir, &oc);
	vec3_dot_vec3(&tmp.z, &oc, &oc);
	tmp = (t_dvec3){
		.x = tmp.x,
		.y = 2 * tmp.y,
		.z = tmp.z - radius * radius, 0.0};
	discriminant_comput(touch, &tmp);
}

void	get_intersect_plane(t_dvec3 *touch, const t_uni *plane,
			const t_ray *ray)
{
	t_dvec3		oc;
	double		div;
	double		oc_dot_dir;

	vec3_dot_vec3(&div, &ray->dir, &plane->dir);
	if (div != 0)
	{
		vec3_sub_vec3(&oc, &ray->pos, &plane->pos);
		vec3_dot_vec3(&oc_dot_dir, &oc, &plane->dir);
		*touch = (t_dvec3) {-oc_dot_dir / div, 0.0, 0.0, 0.0};
	}
	else
	{
		*touch = (t_dvec3){0.0, 0.0, 0.0, 0.0};
	}
}

void	get_intersect_cylinder(t_dvec3 *touch, const t_uni *cylinder,
			const t_ray *ray)
{
	t_dvec3			oc;
	t_dvec3			tmp;
	double			oc_dot_dir;
	double			oc_dot_oc;
	t_dvec3_comp	computs;

	vec3_sub_vec3(&oc, &ray->pos, &cylinder->pos);
	vec3_dot_vec3(&tmp.x, &ray->dir, &ray->dir);
	vec3_dot_vec3(&tmp.y, &ray->dir, &cylinder->dir);
	vec3_dot_vec3(&tmp.z, &ray->dir, &oc);
	vec3_dot_vec3(&oc_dot_dir, &oc, &cylinder->dir);
	vec3_dot_vec3(&oc_dot_oc, &oc, &oc);
	tmp = (t_dvec3){
		.x = tmp.x - (tmp.y * tmp.y),
		.y = 2 * (tmp.z - tmp.y * oc_dot_dir),
		.z = oc_dot_oc - (oc_dot_dir * oc_dot_dir)
			- cylinder->radius * cylinder->radius};
	discriminant_comput(touch, &tmp);
	calculate_oc_tc_dir(&computs, cylinder, ray);
	crop_cyl_n_cone(touch, computs.dir, oc_dot_dir, cylinder->height);
}

void	get_intersect_cone(t_dvec3 *touch, const t_uni *obj, const t_ray *ray)
{
	const double	k = obj->radius / obj->height;
	t_dvec3			tmp;
	double			oc_dot_dir;
	double			oc_dot_oc;
	t_dvec3_comp	computs;

	calculate_oc_tc_dir(&computs, obj, ray);
	vec3_dot_vec3(&tmp.x, &ray->dir, &ray->dir);
	tmp.y = computs.dir;
	vec3_dot_vec3(&tmp.z, &ray->dir, &computs.oc);
	vec3_dot_vec3(&oc_dot_dir, &computs.oc, &obj->dir);
	vec3_dot_vec3(&oc_dot_oc, &computs.oc, &computs.oc);
	tmp = (t_dvec3){
		.x = tmp.x - (1 + k * k) * (tmp.y * tmp.y),
		.y = 2 * (tmp.z - (1 + k * k) * tmp.y * oc_dot_dir),
		.z = oc_dot_oc - (1 + k * k) * (oc_dot_dir * oc_dot_dir)};
	discriminant_comput(touch, &tmp);
	crop_cyl_n_cone(touch, computs.dir, oc_dot_dir, obj->height);
}

void	get_intersect_disk(t_dvec3 *touch, const t_uni *disk, const t_ray *ray)
{
	t_dvec3		v;
	double		v_dot_v;
	t_dvec3		ray_len;
	t_dvec3		touch_point;

	get_intersect_plane(touch, disk, ray);
	if (touch->x != 0.0)
	{
		double_mul_vec3(&ray_len, touch->x, &ray->dir);
		vec3_add_vec3(&touch_point, &ray->pos, &ray_len);
		vec3_sub_vec3(&v, &touch_point, &disk->pos);
		vec3_dot_vec3(&v_dot_v, &v, &v);
		if (v_dot_v > disk->radius * disk->radius)
		{
			*touch = (t_dvec3){0.0, 0.0, 0.0, 0.0};
			return ;
		}
	}
}
