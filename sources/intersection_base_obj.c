/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_base_obj.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 12:56:10 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/04 10:59:00 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void		get_intersect_sphere(const t_uni *sphere, t_dvec3 *ray_pos,
									t_dvec3 *ray_dir, t_dvec3 *touch)
{
	const double		radius = sphere->radius;
	const t_dvec3		oc = vec3_sub_vec3(ray_pos, &sphere->pos);
	t_dvec3				tmp;

	tmp = (t_dvec3){
		.x = vec3_dot_vec3(ray_dir, ray_dir),
		.y = 2 * vec3_dot_vec3(ray_dir, &oc),
		.z = vec3_dot_vec3(&oc, &oc) - radius * radius};
	discriminant_comput(&tmp, touch);
}

void		get_intersect_plane(const t_uni *plane, t_dvec3 *ray_pos,
									t_dvec3 *ray_dir, t_dvec3 *touch)
{
	const t_dvec3		oc = vec3_sub_vec3(ray_pos, &plane->pos);
	double				div = vec3_dot_vec3(ray_dir, &plane->dir);

	if (div != 0)
		*touch = (t_dvec3){-vec3_dot_vec3(&oc, &plane->dir) / div, 0, 0, 0};
	else
		*touch = (t_dvec3){(double)INFINITY, (double)INFINITY, 0.0, 0.0};
}

void 		get_intersect_cylinder(const t_uni *cylinder, t_dvec3 *ray_pos,
									t_dvec3 *ray_dir, t_dvec3 *touch)
{
	const t_dvec3		x = vec3_sub_vec3(ray_pos, &cylinder->pos);
	const t_dvec3		dir = cylinder->dir;
	t_dvec3				tmp;

	tmp = (t_dvec3){.x = vec3_dot_vec3(ray_dir, ray_dir)
					- pow(vec3_dot_vec3(ray_dir, &dir), 2),
					.y = 2 * (vec3_dot_vec3(ray_dir, &x)
						- vec3_dot_vec3(ray_dir, &dir)
							* vec3_dot_vec3(&x, &dir)),
					.z = vec3_dot_vec3(&x, &x)
						- pow(vec3_dot_vec3(&x, &dir), 2)
						- cylinder->radius * cylinder->radius};
	discriminant_comput(&tmp, touch);
}

void		get_intersect_cone(const t_uni *obj, t_dvec3 *ray_pos,
									t_dvec3 *ray_dir, t_dvec3 *touch)
{
	const t_cone		*cone = ((const t_cone *)obj);
	const double		k = cone->angle * M_PI / 360.0;
	const t_dvec3		x = vec3_sub_vec3(ray_pos, &cone->pos);
	const t_dvec3		obj_dir = cone->dir;
	t_dvec3				tmp;

	tmp = (t_dvec3) {
		.x = vec3_dot_vec3(ray_dir, ray_dir) - (1 + k * k)
			* pow(vec3_dot_vec3(ray_dir, &obj_dir), 2),
		.y = 2 * (vec3_dot_vec3(ray_dir, &x)
			- (1 + k * k) * vec3_dot_vec3(ray_dir, &obj_dir)
			* vec3_dot_vec3(&x, &obj_dir)),
		.z = vec3_dot_vec3(&x, &x)
			- (1 + k * k) * pow(vec3_dot_vec3(&x, &obj_dir), 2)};
	discriminant_comput(&tmp, touch);
}
