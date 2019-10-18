/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_down_cam_move.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 11:37:36 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:33:20 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool		is_x_move_down(SDL_Keycode k, t_ivec3 *restrict move)
{
	return ((k == SDLK_a && (move->x = NEG))
		|| (k == SDLK_d && (move->x = POS)));
}

_Bool		is_y_move_down(SDL_Keycode k, t_ivec3 *restrict move)
{
	return ((k == SDLK_s && (move->y = NEG))
		|| (k == SDLK_w && (move->y = POS)));
}

_Bool		is_z_move_down(SDL_Keycode k, t_ivec3 *restrict move)
{
	return ((k == SDLK_q && (move->z = NEG))
		|| (k == SDLK_e && (move->z = POS)));
}
