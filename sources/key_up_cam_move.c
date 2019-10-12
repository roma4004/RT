/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_up_cam_move.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 11:45:40 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/12 13:04:27 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

_Bool		is_x_move_up(SDL_Keycode k, t_ivec3 *restrict move)
{
	return (((k == SDLK_a || k == SDLK_d) && (move->x = NON)));
}

_Bool		is_y_move_up(SDL_Keycode k, t_ivec3 *restrict move)
{
	return (((k == SDLK_s || k == SDLK_w) && (move->y = NON)));
}

_Bool		is_z_move_up(SDL_Keycode k, t_ivec3 *restrict move)
{
	return (((k == SDLK_q || k == SDLK_e) && (move->z = NON)));
}