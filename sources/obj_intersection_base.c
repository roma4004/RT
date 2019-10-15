/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_intersection_base.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 12:56:10 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/15 21:11:36 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	(*intersect_catalog(size_t type))
		(const t_uni *, t_dvec3 *, t_dvec3 *, t_dvec3 *, t_ray *)
{
	if (type == SPHERE)
		return (get_intersect_sphere);
	if (type == PLANE)
		return (get_intersect_plane);
	if (type == CYLINDER)
		return (get_intersect_cylinder);
	if (type == CONE)
		return (get_intersect_cone);
	return (NULL);
}

void get_intersect_sphere(const t_uni *sphere, t_dvec3 *ray_pos, t_dvec3 *ray_dir, t_dvec3 *touch, t_ray *ray)
{
	const double	radius = sphere->radius;
	t_dvec3			oc;
	t_dvec3			tmp;

	vec3_sub_vec3(&oc, ray_pos, &sphere->pos);
	vec3_dot_vec3(&tmp.x, ray_dir, ray_dir);
	vec3_dot_vec3(&tmp.y, ray_dir, &oc);
	vec3_dot_vec3(&tmp.z, &oc, &oc);
	tmp = (t_dvec3){.x = tmp.x, .y = 2 * tmp.y, .z = tmp.z - radius * radius};
	discriminant_comput(&tmp, touch);
}

void get_intersect_plane(const t_uni *plane, t_dvec3 *ray_pos, t_dvec3 *ray_dir, t_dvec3 *touch, t_ray *ray)
{
//todo: add limits
	t_dvec3		oc ;
	double		div;
	double		oc_dot_dir;

	vec3_dot_vec3(&div, ray_dir, &plane->dir);
	if (div != 0)
	{
		vec3_sub_vec3(&oc, ray_pos, &plane->pos);
		vec3_dot_vec3(&oc_dot_dir, &oc, &plane->dir);
		*touch = (t_dvec3) {-oc_dot_dir / div, 0, 0, 0};
	}
	else
	{
		*touch = (t_dvec3){(double)INFINITY, (double)INFINITY, 0.0, 0.0};
	}
}

void get_intersect_cylinder(const t_uni *cylinder, t_dvec3 *ray_pos, t_dvec3 *ray_dir, t_dvec3 *touch, t_ray *ray)
{
	t_dvec3		oc;
	t_dvec3		tmp;
	double		oc_dot_dir;
	double		oc_dot_oc;
	t_dvec3_comp	computs;

	double			height = 1.0;  //parse this value and abs(value)
	vec3_sub_vec3(&oc, ray_pos, &cylinder->pos);
	vec3_dot_vec3(&tmp.x, ray_dir, ray_dir);
	vec3_dot_vec3(&tmp.y, ray_dir, &cylinder->dir);
	vec3_dot_vec3(&tmp.z, ray_dir, &oc);
	vec3_dot_vec3(&oc_dot_dir, &oc, &cylinder->dir);
	vec3_dot_vec3(&oc_dot_oc, &oc, &oc);
	tmp = (t_dvec3){.x = tmp.x - (tmp.y * tmp.y),
					.y = 2 * (tmp.z - tmp.y * oc_dot_dir),
					.z = oc_dot_oc - (oc_dot_dir * oc_dot_dir)
						- cylinder->radius * cylinder->radius};
	discriminant_comput(&tmp, touch);
	vec3_sub_vec3(&computs.oc, &ray->pos, &cylinder->pos);
	vec3_sub_vec3(&computs.tc, &ray->touch_point, &cylinder->pos);
	vec3_dot_vec3(&computs.dir, &ray->dir, &cylinder->dir);
	vec3_dot_vec3(&oc_dot_dir, &computs.oc, &cylinder->dir);
	computs.m = computs.dir * touch->x + oc_dot_dir;
	if (computs.m < 0.0 || computs.m > height)
		touch->x = (double)INFINITY;
	computs.m = computs.dir * touch->y + oc_dot_dir;
	if (computs.m < 0.0 || computs.m > height)
		touch->y = (double)INFINITY;
}

void get_intersect_cone(const t_uni *obj, t_dvec3 *ray_pos, t_dvec3 *ray_dir, t_dvec3 *touch, t_ray *ray)
{
	//todo: add limits (need full refactoring)
	const double	k = ((const t_cone *)obj)->angle * M_PI / 360.0;
	t_dvec3			oc;
	t_dvec3			tmp;
	double			oc_dot_dir;
	double			oc_dot_oc;

	vec3_sub_vec3(&oc, ray_pos, &((const t_cone *)obj)->pos);
	vec3_dot_vec3(&tmp.x, ray_dir, ray_dir);
	vec3_dot_vec3(&tmp.y, ray_dir, &((const t_cone *)obj)->dir);
	vec3_dot_vec3(&tmp.z, ray_dir, &oc);
	vec3_dot_vec3(&oc_dot_dir, &oc, &((const t_cone *)obj)->dir);
	vec3_dot_vec3(&oc_dot_oc, &oc, &oc);
	tmp = (t_dvec3){.x = tmp.x - (1 + k * k) * (tmp.y * tmp.y),
					.y = 2 * (tmp.z - (1 + k * k) * tmp.y * oc_dot_dir),
					.z = oc_dot_oc - (1 + k * k) * (oc_dot_dir * oc_dot_dir)};
	discriminant_comput(&tmp, touch);
}
