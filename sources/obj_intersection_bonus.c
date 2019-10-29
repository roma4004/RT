/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_intersection_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 17:36:43 by vtlostiu          #+#    #+#             */
/*   Updated: 2019/10/29 14:08:11 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	get_intersect_paraboloid(t_dvec3 *touch, const t_uni *paraboloid,
							const t_ray *ray)
{
	t_dvec3			oc;
	t_dvec3			tmp;
	double			oc_dot_dir;
	double			oc_dot_oc;
	t_dvec3_comp	computs;

	vec3_sub_vec3(&oc, &ray->pos, &paraboloid->pos);
	vec3_dot_vec3(&tmp.x, &ray->dir, &ray->dir);
	vec3_dot_vec3(&tmp.y, &ray->dir, &paraboloid->dir);
	vec3_dot_vec3(&tmp.z, &ray->dir, &oc);
	vec3_dot_vec3(&oc_dot_dir, &oc, &paraboloid->dir);
	vec3_dot_vec3(&oc_dot_oc, &oc, &oc);
	tmp = (t_dvec3){
			.x = tmp.x - pow(tmp.y, 2),
			.y = 2 * (tmp.z - tmp.y * (oc_dot_dir + 2 * paraboloid->radius)),
			.z = oc_dot_oc - oc_dot_dir *
					(oc_dot_dir + 4 * paraboloid->radius)};
	discriminant_comput(touch, &tmp);
	calculate_oc_tc_dir(&computs, paraboloid, ray);
	crop_cyl_n_cone(touch, computs.dir, oc_dot_dir, paraboloid->height);
}