/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_normal_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 12:45:26 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/15 18:10:28 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		get_normal_sphere(t_ray *ray, const t_uni *obj,
									double dist, t_dvec3 *normal)
{
	t_dvec3		tc;

	(void)dist;
	vec3_sub_vec3(&tc, &ray->touch_point, &obj->pos);
	vec3_normalize(normal, &tc);
}

static void		get_normal_plane(t_ray *ray, const t_uni *obj,
									double dist, t_dvec3 *normal)
{
	double		dir_dot;
	t_dvec3		dir_vec;
	t_dvec3		dir_vec_inv;

	(void)dist;
	vec3_dot_vec3(&dir_dot, &ray->dir, &obj->dir);
	if (dir_dot > 0)
	{
		double_mul_vec3(&dir_vec_inv, -1, &obj->dir);
		dir_vec = dir_vec_inv;
	}
	else
	{
		dir_vec = obj->dir;
	}
	vec3_normalize(normal, &dir_vec);
}

static void		get_normal_cylinder(t_ray *ray, const t_uni *obj,
									double dist, t_dvec3 *normal)
{
	t_dvec3_comp	computs;
	double			oc_dot_dir;

	vec3_sub_vec3(&computs.oc, &ray->pos, &obj->pos);
	vec3_sub_vec3(&computs.tc, &ray->touch_point, &obj->pos);
	vec3_dot_vec3(&computs.dir, &ray->dir, &obj->dir);
	vec3_dot_vec3(&oc_dot_dir, &computs.oc, &obj->dir);
	computs.m = computs.dir * dist + oc_dot_dir;
	vec3_mul_double(&computs.dir_vec, &obj->dir, computs.m);
	vec3_sub_vec3(&computs.dir_vec, &computs.tc, &computs.dir_vec);
	vec3_normalize(normal, &computs.dir_vec);
}

static void		get_normal_cone(t_ray *ray, const t_uni *obj,
								double dist, t_dvec3 *normal)
{
	const double	k = ((const t_cone *)obj)->angle * M_PI / 360.0;
	t_dvec3_comp	computs;
	double			oc_dot_dir;

	vec3_sub_vec3(&computs.tc, &ray->touch_point, &obj->pos);
	vec3_sub_vec3(&computs.oc, &ray->pos, &obj->pos);
	vec3_dot_vec3(&computs.dir, &ray->dir, &obj->dir);
	vec3_dot_vec3(&oc_dot_dir, &computs.oc, &obj->dir);
	computs.m = computs.dir * dist + oc_dot_dir;
	double_mul_vec3(&computs.dir_vec, (1 + k * k), &obj->dir);
	vec3_mul_double(&computs.dir_vec, &computs.dir_vec, computs.m);
	vec3_sub_vec3(&computs.dir_vec, &computs.tc, &computs.dir_vec);
	vec3_normalize(normal, &computs.dir_vec);
}

void				(*normal_catalog(size_t type))
						(t_ray *, const t_uni *, double, t_dvec3 *)
{
	if (type == SPHERE)
		return (get_normal_sphere);
	if (type == PLANE)
		return (get_normal_plane);
	if (type == CYLINDER)
		return (get_normal_cylinder);
	if (type == CONE)
		return (get_normal_cone);
	return (NULL);
}
