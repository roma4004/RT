/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_mouse_move.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 15:36:02 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/20 15:37:51 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool		mouse_move(t_env *env, SDL_Event *event)
{
	if (event->button.button == SDL_BUTTON_LEFT)
	{
		if (event->motion.xrel < -1)
			env->flags.move.x = POS;
		else if (event->motion.xrel > 1)
			env->flags.move.x = NEG;
		if (event->motion.yrel < -1)
			env->flags.move.y = POS;
		else if (event->motion.yrel > 1)
			env->flags.move.y = NEG;
		return (true);
	}
	return (false);
}

_Bool		mouse_rotate(t_env *env, SDL_Event *event)
{
	if (event->button.button == SDL_BUTTON_X1)
	{
		if (event->motion.xrel < -1)
			env->flags.rotate.x = NEG;
		else if (event->motion.xrel > 1)
			env->flags.rotate.x = POS;
		if (event->motion.yrel < -1)
			env->flags.rotate.y = POS;
		else if (event->motion.yrel > 1)
			env->flags.rotate.y = NEG;
		return (true);
	}
	return (false);
}

_Bool		mouse_rotate_z(t_env *env, SDL_Event *event)
{
	if (event->button.button == SDL_BUTTON_MIDDLE)
	{
		if (event->motion.xrel < -2)
			env->flags.rotate.z = NEG;
		else if (event->motion.xrel > 2)
			env->flags.rotate.z = POS;
		return (true);
	}
	return (false);
}