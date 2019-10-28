/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:51:21 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/28 20:50:43 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		show_legend(t_env *env, t_ivec *pos, t_ivec offset)
{
	pos->x += 30;
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "position:");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "radius:");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "direction:");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "height:");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "col:");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "specular:");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "reflective:");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "refractive:");
	pos->x -= 30;
}

static void		show_vec(t_env *env, t_ivec pos, t_dvec3 *vec, t_ivec offset)
{
	free(draw_text(env, (t_ivec){pos.x, pos.y},
		ft_itoa((int)vec->x)));
	free(draw_text(env, (t_ivec){pos.x += offset.x, pos.y},
		ft_itoa((int)vec->y)));
	free(draw_text(env, (t_ivec){pos.x += offset.x, pos.y},
		ft_itoa((int)vec->z)));
}

static void		show_value(t_env *env, t_ivec *pos, t_ivec line_offset)
{
	t_uni	*obj;

	if (!env)
		return ;
	if (!(obj = env->selected_obj))
		return ;
	show_vec(env, (t_ivec) {pos->x, pos->y}, &obj->pos, line_offset);
	free(draw_text(env, (t_ivec) {pos->x, pos->y += line_offset.y},
		ft_itoa((int)obj->radius)));
	show_vec(env, (t_ivec) {pos->x, pos->y += line_offset.y},
		&obj->dir, line_offset);
	free(draw_text(env, (t_ivec) {pos->x, pos->y += line_offset.y},
		ft_itoa((int)obj->height)));
	show_vec(env, (t_ivec) {pos->x, pos->y += line_offset.y},
		&obj->color, line_offset);
	free(draw_text(env, (t_ivec) {pos->x, pos->y += line_offset.y},
		ft_itoa((int)obj->specular)));
	free(draw_text(env, (t_ivec) {pos->x, pos->y += line_offset.y},
		ft_itoa((int)obj->reflective_coef)));
	free(draw_text(env, (t_ivec) {pos->x, pos->y += line_offset.y},
		ft_itoa((int)obj->refractive_coef)));
}

void			show_info(t_env *env, t_ivec *pos, t_ivec offset)
{
	t_ivec	val;

	draw_text(env, (t_ivec){pos->x, pos->y}, "object info:");
	show_legend(env, pos, offset);
	val = (t_ivec){200, 20 + offset.y};
	show_value(env, &val, offset);
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y},
		"hide object info: I");
}
