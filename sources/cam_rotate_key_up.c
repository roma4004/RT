/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cam_rotate_key_up.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 11:42:31 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/02 11:47:32 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

_Bool			is_x_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return (((k == SDLK_t || k == SDLK_g) && (rotate->x = NON)));
}

_Bool			is_y_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return (((k == SDLK_y || k == SDLK_h) && (rotate->y = NON)));
}

_Bool			is_z_rotate_up(SDL_Keycode k, t_ivec3 *restrict rotate)
{
	return (((k == SDLK_u || k == SDLK_j) && (rotate->z = NON)));
}