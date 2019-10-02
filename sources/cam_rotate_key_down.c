/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cam_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 11:38:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/02 11:38:38 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

_Bool			is_x_rotate_down(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return ((k == SDLK_t && (rotate->x = NEG))
		|| (k == SDLK_g && (rotate->x = POS)));
}

_Bool			is_y_rotate_down(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return ((k == SDLK_y && (rotate->y = NEG))
		|| (k == SDLK_h && (rotate->y = POS)));
}

_Bool			is_z_rotate_down(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return ((k == SDLK_u && (rotate->z = NEG))
		|| (k == SDLK_j && (rotate->z = POS)));
}
