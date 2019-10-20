/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_normal_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/01 12:45:26 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 17:38:33 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	set_normal_sphere(t_ray *ray, const t_uni *plane, double dist)
{
	t_dvec3		tc;

	(void)dist;
	vec3_sub_vec3(&tc, &ray->touch_point, &plane->pos);
	vec3_normalize(&ray->normal, &tc);
}

void	set_normal_plane(t_ray *ray, const t_uni *plane, double dist)
{
	double		dir_dot;
	t_dvec3		dir_vec;
	t_dvec3		dir_vec_inv;

	(void)dist;
	vec3_dot_vec3(&dir_dot, &ray->dir, &plane->dir);
	if (dir_dot > 0)
	{
		double_mul_vec3(&dir_vec_inv, -1, &plane->dir);
		dir_vec = dir_vec_inv;
	}
	else
	{
		dir_vec = plane->dir;
	}
	vec3_normalize(&ray->normal, &dir_vec);
}

void	set_normal_cylinder(t_ray *ray, const t_uni *cylinder, double dist)
{
	t_dvec3_comp	computs;
	double			oc_dot_dir;

	calculate_oc_tc_dir(&computs, cylinder, ray);
	vec3_dot_vec3(&oc_dot_dir, &computs.oc, &cylinder->dir);
	computs.m = computs.dir * dist + oc_dot_dir;
	vec3_mul_double(&computs.dir_vec, &cylinder->dir, computs.m);
	vec3_sub_vec3(&computs.dir_vec, &computs.tc, &computs.dir_vec);
	vec3_normalize(&ray->normal, &computs.dir_vec);
}

void	set_normal_cone(t_ray *ray, const t_uni *cone, double dist)
{
	const double	k = cone->radius / cone->height;
	t_dvec3_comp	computs;
	double			oc_dot_dir;

	calculate_oc_tc_dir(&computs, cone, ray);
	vec3_dot_vec3(&oc_dot_dir, &computs.oc, &cone->dir);
	computs.m = computs.dir * dist + oc_dot_dir;
	double_mul_vec3(&computs.dir_vec, (1 + k * k), &cone->dir);
	vec3_mul_double(&computs.dir_vec, &computs.dir_vec, computs.m);
	vec3_sub_vec3(&computs.dir_vec, &computs.tc, &computs.dir_vec);
	vec3_normalize(&ray->normal, &computs.dir_vec);
}
