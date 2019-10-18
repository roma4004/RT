/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_up_cam_rotate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 11:42:31 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:33:20 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool		is_x_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return (((k == SDLK_t || k == SDLK_g) && (rotate->x = NON)));
}

_Bool		is_y_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return (((k == SDLK_y || k == SDLK_h) && (rotate->y = NON)));
}

_Bool		is_z_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return (((k == SDLK_u || k == SDLK_j) && (rotate->z = NON)));
}