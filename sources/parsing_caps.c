/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_caps.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 15:30:31 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 17:24:53 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			calc_bot_cap(t_uni *cap, t_uni *his_parent)
{
	cap->pos = his_parent->pos;
}

void			calc_top_cap(t_uni *cap, t_uni *his_parent)
{
	vec3_mul_double(&cap->pos, &his_parent->dir, his_parent->height);
	vec3_add_vec3(&cap->pos, &his_parent->pos, &cap->pos);
}

static void		add_cap_bottom(t_uni *dest_child, t_uni *src_parent)
{
	*dest_child = (t_uni){
		.pos = src_parent->pos,
		.radius = src_parent->radius,
		.color = src_parent->color,
		.specular = src_parent->specular,
		.get_intersect = g_intersect_catalog(DISK),
		.get_normal = g_normal_catalog(DISK),
		.reflective_coef = src_parent->reflective_coef,
		.refractive_coef = src_parent->refractive_coef,
		.is_selected = src_parent->is_selected,
	};
	vec3_mul_double(&dest_child->dir, &src_parent->dir, -1);
	vec3_normalize(&dest_child->dir, &dest_child->dir);
	set_backup_val(dest_child);
}

static void		add_cap_top(t_uni *dest_child, t_uni *src_parent)
{
	*dest_child = (t_uni){
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
	calc_top_cap(dest_child, src_parent);
	set_backup_val(dest_child);
}

void			add_caps(t_uni *uni_arr, size_t *id_uni, size_t type)
{
	t_uni		*src_parent;

	if (type == CYLINDER )
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
