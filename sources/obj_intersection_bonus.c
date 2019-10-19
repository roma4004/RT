/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_intersection_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 17:36:43 by vtlostiu          #+#    #+#             */
/*   Updated: 2019/10/19 20:16:55 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
//
//void	get_intersect_ellipsoid(const t_uni *obj, t_dvec3 *touch, t_ray *ray)
//{
////	const double	radius = sphere->radius;
//	t_dvec3			oc;
//	t_dvec3			tmp;
//	double		oc_dot_oc;
//
//	double k = 2.0;
//	double r = obj->radius;
//	double		oc_dot_dir;
//
//
//	double DV;
//	double DD;
//	double	DX;
//
//
////	V - obj->dir
////	D - ray->dir
////  D|D = 	vec3_dot_vec3(&tmp.x, &ray->dir, &ray->dir);
////  D|X = vec3_dot_vec3(&tmp.z, &ray->dir, &oc);
////  X|X = oc_dot_oc;
//
//
//	vec3_sub_vec3(&oc, &ray->pos, &obj->pos);   //?
//	vec3_dot_vec3(&oc_dot_dir, &oc, &obj->dir);
//	vec3_dot_vec3(&oc_dot_oc, &oc, &oc);
//
//
//	vec3_dot_vec3(&DV, &ray->dir, &obj->dir);
//	double A1 = 2 * k * DV;
//	double A2  = r * r + 2 * k * oc_dot_dir - k;
//
//
//
//	vec3_dot_vec3(&DD, &ray->dir, &ray->dir);
//	vec3_dot_vec3(&DX, &ray->dir, &oc);
////  X|X = oc_dot_oc;
//
//	tmp = (t_dvec3){
//			.x =      4 * r * r * DD - A1 * A1,
//			.y = 2 * (4 * r * r * DX - A1 * A2),
//			.z = 4 * r * r * oc_dot_oc - A2 * A2};
//	discriminant_comput(&tmp, touch);
//}