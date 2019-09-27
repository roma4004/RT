/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uni_intersection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 12:56:10 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/27 19:17:46 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void			intersect_sphere(t_dvec3 *ray_pos, t_uni *obj,
								t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	const t_dvec3		center = obj->pos;
	const double		radius = obj->radius;
	t_dvec3				oc;
	t_dvec3				tmp;

	oc = vec3_sub_vec3(*ray_pos, center);

	tmp = (t_dvec3) {
		.x = vec3_dot_vec3(ray_dir, ray_dir),
		.y = vec3_to_double(double_mul_vec3(2, vec3_mul_vec3(oc, ray_dir))),
		.z = vec3_dot_vec3(oc, oc) - radius * radius};
	*plane_toch = discriminant_comput(&tmp);
}

void			intersect_plane(t_dvec3 *ray_pos, t_uni *plane,
								t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	const t_dvec3	normal = vec3_normalize(plane->dir);
	const double	denom = vec3_dot_vec3(normal, ray_dir); //need norm dir after parsing

	if (denom > 1e-6 || denom < 1e-6)
		*plane_toch = (t_dvec3){
			vec3_dot_vec3(vec3_sub_vec3(plane->pos, *ray_pos), normal) / denom,
			MAXFLOAT,
			0};
}

void		intersect_cylinder(t_dvec3 *ray_pos, t_uni *obj,
								t_dvec3 ray_dir, t_dvec3 *plane_toch)
{
	t_dvec3 x = vec3_sub_vec3(*ray_pos, obj->pos);
	t_dvec3 tmp;
	//	D - ray direction.
	//	V is a unit length vector that determines cylinder's axis
	//	x equals O-C.
	//	O is the ray origin
	//	C is a center point of a shape that we hit

	//	a   = D|D - (D|V)^2
	//	b/2 = D|x - (D|V)*(x|V)
	//	c   = x|x - (x|V)^2 - r*r
	t_dvec3 v = vec3_normalize(obj->dir); //need norm dir after parsing
	tmp = (t_dvec3){.x = vec3_dot_vec3(ray_dir, ray_dir)
		- pow(vec3_dot_vec3(ray_dir, v), 2),
		.y = 2 * (vec3_dot_vec3(ray_dir, x)
			- vec3_dot_vec3(ray_dir, v)
				* vec3_dot_vec3(x, v)),
		.z = vec3_dot_vec3(x, x)
			- pow(vec3_dot_vec3(x, v), 2)
			- obj->radius * obj->radius};
	*plane_toch = (discriminant_comput(&tmp));
}

void		intersect_cone(t_dvec3 *ray_pos, t_uni *obj,
							t_dvec3 ray_dir, t_dvec3 *plane_toch)
{//	const double	k = tan(obj->radius / 2.0); //radius is a angle on top of the cone
	const double	k = ((t_cone *) obj)->angle * M_PI / 360.0; //radius is a angle on top of the cone
	const t_dvec3	x = vec3_sub_vec3(*ray_pos, ((t_cone *) obj)->pos);
	const t_dvec3	obj_dir = vec3_normalize(((t_cone *) obj)->dir); //need norm dir after parsing
	t_dvec3			tmp;

	tmp = (t_dvec3) {
		.x = vec3_dot_vec3(ray_dir, ray_dir) - (1 + k * k)
			* pow(vec3_dot_vec3(ray_dir, obj_dir), 2),
		.y = 2 * (vec3_dot_vec3(ray_dir, x)
			- (1 + k * k) * vec3_dot_vec3(ray_dir, obj_dir)
			* vec3_dot_vec3(x, obj_dir)),
		.z = vec3_dot_vec3(x, x)
			- (1 + k * k) * pow(vec3_dot_vec3(x, obj_dir), 2)};
	*plane_toch = (discriminant_comput(&tmp));
	//	dir - ray direction.
	//	obj_dir is a unit length vector that determines cylinder's axis
	//	x equals O-C.
	//	O is the ray ray_pos
	//	C is a center ray_pos of a shape that we hit
}
