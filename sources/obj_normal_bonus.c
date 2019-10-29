/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_normal_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 19:45:02 by vtlostiu          #+#    #+#             */
/*   Updated: 2019/10/29 19:45:02 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	set_normal_paraboloid(t_ray *ray, const t_uni *paraboloid, double dist)
{
	t_dvec3_comp	computs;
	double			oc_dot_dir;
	double			tmp;

	calculate_oc_tc_dir(&computs, paraboloid, ray);
	vec3_dot_vec3(&oc_dot_dir, &computs.oc, &paraboloid->dir);
	computs.m = computs.dir * dist + oc_dot_dir;
	tmp = computs.m + (paraboloid->radius / 100);
	vec3_mul_double(&computs.dir_vec, &paraboloid->dir, tmp);
	vec3_sub_vec3(&computs.dir_vec, &computs.tc, &computs.dir_vec);
	vec3_normalize(&ray->normal, &computs.dir_vec);
}
