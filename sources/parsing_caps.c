/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_caps.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 15:30:31 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/20 15:34:15 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		add_cap_bottom(t_uni *dest_child, t_uni *src_parent)
{
	t_dvec3		cap_view;

	vec3_mul_double(&cap_view, &src_parent->dir, -1);
	vec3_normalize(&cap_view, &cap_view);
	*dest_child = (t_uni){
		.pos = src_parent->pos,
		.radius = src_parent->radius,
		.dir = cap_view,
		.color = src_parent->color,
		.specular = src_parent->specular,
		.get_intersect = g_intersect_catalog(DISK),
		.get_normal = g_normal_catalog(DISK),
		.reflective_coef = src_parent->reflective_coef,
		.refractive_coef = src_parent->refractive_coef,
		.is_selected = src_parent->is_selected,
	};
	set_backup_val(dest_child);
}

static void		add_cap_top(t_uni *dest_child, t_uni *src_parent)
{
	t_dvec3		cap_top_pos;

	vec3_mul_double(&cap_top_pos, &src_parent->dir, src_parent->height);
	vec3_add_vec3(&cap_top_pos, &src_parent->pos, &cap_top_pos);
	*dest_child = (t_uni){
		.pos = cap_top_pos,
		.radius = src_parent->radius,
		.dir = src_parent->dir,
		.color = src_parent->color,
		.specular = src_parent->specular,
		.get_intersect = g_intersect_catalog(DISK),
		.get_normal = g_normal_catalog(DISK),
		.reflective_coef = src_parent->reflective_coef,
		.refractive_coef = src_parent->refractive_coef,
		.is_selected = src_parent->is_selected,
	};
	set_backup_val(dest_child);
}

void			add_caps(t_uni *uni_arr, size_t *id_uni, size_t type)
{
	t_uni		*src_parent;

	if (type == CYLINDER)
	{
		src_parent = &uni_arr[*id_uni];
		(*id_uni)++;
		add_cap_bottom(&(uni_arr[*id_uni]), src_parent);
		(*id_uni)++;
		add_cap_top(&(uni_arr[*id_uni]), src_parent);
	}
	if (type == CONE)
	{
		src_parent = &uni_arr[*id_uni];
		(*id_uni)++;
		add_cap_top(&(uni_arr[*id_uni]), src_parent);
	}
}
