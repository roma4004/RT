/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 13:50:28 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 20:50:50 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	discriminant_comput(t_dvec3 *touch, const t_dvec3 *tmp)
{
	const double	discriminant = tmp->y * tmp->y - 4 * tmp->x * tmp->z;

	if (discriminant < 0.0)
		*touch = (t_dvec3){0.0, 0.0, 0.0, 0.0};
	else
	{
		*touch = (t_dvec3) {
			.x = (-tmp->y + sqrt(discriminant)) / (2.0 * tmp->x),
			.y = (-tmp->y - sqrt(discriminant)) / (2.0 * tmp->x),
			.z = 0.0,
			0.0};
	}
}

void	(*g_intersect_catalog(uint64_t type))(t_dvec3 *touch, const t_uni *obj,
			const t_ray *ray)
{
	if (type == SPHERENEG || type == SPHERE)
		return (get_intersect_sphere);
	if (type == PLANENEG || type == PLANE)
		return (get_intersect_plane);
	if (type == CYLINDERNEG || type == CYLINDER)
		return (get_intersect_cylinder);
	if (type == CONENEG || type == CONE)
		return (get_intersect_cone);
	if (type == DISKNEG || type == DISK)
		return (get_intersect_disk);
	if (type == PARABOLOIDNEG || type == PARABOLOID)
		return (get_intersect_paraboloid);
	return (NULL);
}

void	(*g_normal_catalog(uint64_t type))(t_ray *ray, const t_uni *obj,
			double dist)
{
	if (type == SPHERENEG || type == SPHERE)
		return (set_normal_sphere);
	if (type == PLANENEG || type == PLANE
	|| type == DISKNEG || type == DISK)
		return (set_normal_plane);
	if (type == CYLINDERNEG || type == CYLINDER)
		return (set_normal_cylinder);
	if (type == CONENEG || type == CONE)
		return (set_normal_cone);
	if (type == PARABOLOIDNEG || type == PARABOLOID)
		return (set_normal_paraboloid);
	return (NULL);
}

void	crop_cyl_n_cone(t_dvec3 *touch, double dir, double oc_dot_dir,
			double height)
{
	double		m;

	m = dir * touch->x + oc_dot_dir;
	if (m < 0.0 || m > height)
		touch->x = 0.0;
	m = dir * touch->y + oc_dot_dir;
	if (m < 0.0 || m > height)
		touch->y = 0.0;
}

void	calculate_oc_tc_dir(t_dvec3_comp *computs, const t_uni *obj,
			const t_ray *ray)
{
	vec3_sub_vec3(&computs->oc, &ray->pos, &obj->pos);
	vec3_sub_vec3(&computs->tc, &ray->touch_point, &obj->pos);
	vec3_dot_vec3(&computs->dir, &ray->dir, &obj->dir);
}
