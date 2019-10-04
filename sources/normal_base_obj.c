/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_base_obj.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 12:45:26 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/04 12:42:13 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		get_normal_sphere(t_ray *ray, const t_uni *obj,
									double dist, t_dvec3 *normal)
{
	const t_dvec3 tc = vec3_sub_vec3(&ray->touch_point, &obj->pos);

	(void)dist;
	vec3_normalize(normal, &tc);
}

static void		get_normal_plane(t_ray *ray, const t_uni *obj,
									double dist, t_dvec3 *normal)
{
	const double	dir_dot = vec3_dot_vec3(&ray->dir, &obj->dir);
	t_dvec3			dir_vec;

	(void)dist;
	dir_vec = dir_dot > 0 ? vec3_mul_double(&obj->dir, -1) : obj->dir;
	vec3_normalize(normal, &dir_vec);
}

static void		get_normal_cylinder(t_ray *ray, const t_uni *obj,
									double dist, t_dvec3 *normal)
{
	t_dvec3_comp	computs;

	computs.oc = vec3_sub_vec3(&ray->pos, &obj->pos);
	computs.tc = vec3_sub_vec3(&ray->touch_point, &obj->pos);
	computs.dir = vec3_dot_vec3(&ray->dir, &obj->dir);
	computs.m = computs.dir * dist + vec3_dot_vec3(&computs.oc, &obj->dir);
	computs.dir_vec = vec3_mul_double(&obj->dir, computs.m);
	computs.dir_vec = vec3_sub_vec3(&computs.tc, &computs.dir_vec);
	vec3_normalize(normal, &computs.dir_vec);
}

static void		get_normal_cone(t_ray *ray, const t_uni *obj,
								double dist, t_dvec3 *normal)
{
	const double	k = ((const t_cone *)obj)->angle * M_PI / 360.0;
	t_dvec3_comp	computs;

	computs.tc = vec3_sub_vec3(&ray->touch_point, &obj->pos);
	computs.oc = vec3_sub_vec3(&ray->pos, &obj->pos);
	computs.dir = vec3_dot_vec3(&ray->dir, &obj->dir);
	computs.m = computs.dir * dist + vec3_dot_vec3(&computs.oc, &obj->dir);
	computs.dir_vec = double_mul_vec3((1 + k * k), &obj->dir);
	computs.dir_vec = vec3_mul_double(&computs.dir_vec, computs.m);
	computs.dir_vec = vec3_sub_vec3(&computs.tc, &computs.dir_vec);
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
