/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_mouse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 15:36:02 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/30 00:37:30 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool		mouse_move(t_env *env, SDL_Event *event)
{
	if (event->button.button == SDL_BUTTON_LEFT)
	{
		if (event->motion.xrel < -2)
			env->flags.move.x = NEG * 2;
		else if (event->motion.xrel > 2)
			env->flags.move.x = POS * 2;
		if (event->motion.yrel < -2)
			env->flags.move.y = POS * 2;
		else if (event->motion.yrel > 2)
			env->flags.move.y = NEG * 2;
		if (event->motion.xrel || event->motion.yrel)
			return (true);
	}
	return (false);
}

_Bool		mouse_rotate(t_env *env, SDL_Event *event)
{
	if (event->button.button == SDL_BUTTON_X1)
	{
		if (event->motion.xrel < -2)
			env->flags.rotate.x = NEG * 2;
		else if (event->motion.xrel > 2)
			env->flags.rotate.x = POS * 2;
		if (event->motion.yrel < -2)
			env->flags.rotate.y = POS * 2;
		else if (event->motion.yrel > 2)
			env->flags.rotate.y = NEG * 2;
		if (event->motion.xrel || event->motion.yrel)
			return (true);
	}
	return (false);
}

_Bool		mouse_rotate_z(t_env *env, SDL_Event *event)
{
	if (event->button.button == SDL_BUTTON_MIDDLE)
	{
		if (event->motion.xrel < 0)
			env->flags.rotate.z = NEG * 2;
		else if (event->motion.xrel > 0)
			env->flags.rotate.z = POS * 2;
		if (event->motion.xrel)
			return (true);
	}
	return (false);
}
