/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_up_cam_rotate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 11:42:31 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 14:55:21 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool		is_x_rotate_up(t_ivec3 *rotate, SDL_Keycode k)
{
	return (((k == SDLK_t || k == SDLK_g) && (rotate->x = NON)));
}

_Bool		is_y_rotate_up(t_ivec3 *rotate, SDL_Keycode k)
{
	return (((k == SDLK_y || k == SDLK_h) && (rotate->y = NON)));
}

_Bool		is_z_rotate_up(t_ivec3 *rotate, SDL_Keycode k)
{
	return (((k == SDLK_u || k == SDLK_j) && (rotate->z = NON)));
}
