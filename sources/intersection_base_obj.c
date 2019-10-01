/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uni_intersection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 12:56:10 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 12:41:38 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void		get_intersect_sphere(const t_uni *obj, t_dvec3 *ray_pos,
									t_dvec3 *ray_dir, t_dvec3 *touch)
{
	const t_dvec3		center = obj->pos;
	const double		radius = obj->radius;
	t_dvec3				oc;
	t_dvec3				tmp;

	oc = vec3_sub_vec3(*ray_pos, center);
	tmp = (t_dvec3) {
		.x = vec3_dot_vec3(*ray_dir, *ray_dir),
		.y = vec3_to_double(double_mul_vec3(2, vec3_mul_vec3(oc, *ray_dir))),
		.z = vec3_dot_vec3(oc, oc) - radius * radius};
	discriminant_comput(&tmp, touch);
}

void		get_intersect_plane(const t_uni *plane, t_dvec3 *ray_pos,
									t_dvec3 *ray_dir, t_dvec3 *touch)
{
	const t_dvec3	normal = plane->dir;
	const double	denom = vec3_dot_vec3(normal, *ray_dir);

	if (denom > 1e-6 || denom < 1e-6)
		*touch = (t_dvec3){
			vec3_dot_vec3(vec3_sub_vec3(plane->pos, *ray_pos), normal) / denom,
			(double)MAXFLOAT,
			0, 0};
}

void 		get_intersect_cylinder(const t_uni *obj, t_dvec3 *ray_pos,
									t_dvec3 *ray_dir, t_dvec3 *touch)
{
	const t_dvec3		x = vec3_sub_vec3(*ray_pos, obj->pos);
	const t_dvec3		dir = obj->dir;
	t_dvec3				tmp;

	tmp = (t_dvec3){.x = vec3_dot_vec3(*ray_dir, *ray_dir)
					- pow(vec3_dot_vec3(*ray_dir, dir), 2),
					.y = 2 * (vec3_dot_vec3(*ray_dir, x)
						- vec3_dot_vec3(*ray_dir, dir)
							* vec3_dot_vec3(x, dir)),
					.z = vec3_dot_vec3(x, x)
						- pow(vec3_dot_vec3(x, dir), 2)
						- obj->radius * obj->radius};
	discriminant_comput(&tmp, touch);
}

void		get_intersect_cone(const t_uni *obj, t_dvec3 *ray_pos,
									t_dvec3 *ray_dir, t_dvec3 *touch)
{
	const double		k = ((const t_cone *)obj)->angle * M_PI / 360.0;
	const t_dvec3		x = vec3_sub_vec3(*ray_pos, ((const t_cone *)obj)->pos);
	const t_dvec3		obj_dir = ((const t_cone *)obj)->dir;
	t_dvec3				tmp;

	tmp = (t_dvec3) {
		.x = vec3_dot_vec3(*ray_dir, *ray_dir) - (1 + k * k)
			* pow(vec3_dot_vec3(*ray_dir, obj_dir), 2),
		.y = 2 * (vec3_dot_vec3(*ray_dir, x)
			- (1 + k * k) * vec3_dot_vec3(*ray_dir, obj_dir)
			* vec3_dot_vec3(x, obj_dir)),
		.z = vec3_dot_vec3(x, x)
			- (1 + k * k) * pow(vec3_dot_vec3(x, obj_dir), 2)};
	discriminant_comput(&tmp, touch);
}
