/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_up_cam_move.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 11:45:40 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:33:20 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool		is_x_move_up(t_ivec3 *move, SDL_Keycode k)
{
	return (((k == SDLK_a || k == SDLK_d) && (move->x = NON)));
}

_Bool		is_y_move_up(t_ivec3 *move, SDL_Keycode k)
{
	return (((k == SDLK_s || k == SDLK_w) && (move->y = NON)));
}

_Bool		is_z_move_up(t_ivec3 *move, SDL_Keycode k)
{
	return (((k == SDLK_q || k == SDLK_e) && (move->z = NON)));
}
