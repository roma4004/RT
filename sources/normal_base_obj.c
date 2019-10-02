/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_base_obj.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 12:45:26 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/02 17:51:30 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		get_normal_sphere(t_ray *ray, const t_uni *obj,
								double dist, t_dvec3 *normal)
{
	(void)ray;
	(void)dist;
	*normal = vec3_normalize(vec3_sub_vec3(ray->touch_point, obj->pos));
}

static void		get_normal_plane(t_ray *ray, const t_uni *obj,
								double dist, t_dvec3 *normal)
{
	(void)ray;
	(void)dist;
	*normal = vec3_normalize(
							vec3_dot_vec3(ray->dir, obj->dir) > 0
							? vec3_mul_double(obj->dir, -1)
							: obj->dir);
}

static void		get_normal_cylinder(t_ray *ray, const t_uni *obj,
								double dist, t_dvec3 *normal)
{
	(void)ray;
	*normal = vec3_normalize(
				vec3_sub_vec3(
					vec3_sub_vec3(ray->touch_point, obj->pos),
					vec3_mul_double(obj->dir,
						vec3_dot_vec3(ray->dir, obj->dir)
						* dist
						+ vec3_dot_vec3(vec3_sub_vec3(ray->pos, obj->pos),
							obj->dir)
					)
				)
			);
}

static void		get_normal_cone(t_ray *ray, const t_uni *obj,
								double dist, t_dvec3 *normal)
{
	const t_cone	*cone = (const t_cone *)obj;
	double			k;

	k = cone->angle * M_PI / 360.0;
	*normal = vec3_normalize(
				vec3_sub_vec3(vec3_sub_vec3(ray->touch_point, obj->pos),
					vec3_mul_double(double_mul_vec3((1 + k * k), obj->dir),
						vec3_dot_vec3(ray->dir, obj->dir)
						* dist
						+ vec3_dot_vec3(vec3_sub_vec3(ray->pos, obj->pos),
										obj->dir)
					)
				)
			);
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
