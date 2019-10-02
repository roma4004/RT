/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/02 11:47:32 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		keyboard_handle(t_cam *restrict cam, t_flags *restrict f,
								double move_speed, double rotate_speed)
{
	cam->pos.x += f->move.x * move_speed;
	cam->pos.y += f->move.y * move_speed;
	cam->pos.z += f->move.z * move_speed;
	cam->rotate_angle.x += f->rotate.x * rotate_speed;
	cam->rotate_angle.y += f->rotate.y * rotate_speed;
	cam->rotate_angle.z += f->rotate.z * rotate_speed;
}

static _Bool	keyboard_evens(Uint32 event_type, SDL_Keycode k,
								t_flags *restrict f)
{
	if (event_type == SDL_KEYDOWN && (is_x_move_down(k, &f->move)
									|| is_y_move_down(k, &f->move)
									|| is_z_move_down(k, &f->move)
									|| is_x_rotate_down(k, &f->rotate)
									|| is_y_rotate_down(k, &f->rotate)
									|| is_z_rotate_down(k, &f->rotate)))
		return (true);
	if (event_type == SDL_KEYUP && (is_x_move_up(k, &f->move)
								|| is_y_move_up(k, &f->move)
								|| is_z_move_up(k, &f->move)
								|| is_x_rotate_up(k, &f->rotate)
								|| is_y_rotate_up(k, &f->rotate)
								|| is_z_rotate_up(k, &f->rotate)))
		return (false);
	return (false);
}

_Bool			event_handler(t_cam *cam, t_flags *flags)
{
	SDL_Event			event;
	SDL_Keycode			key_code;
	unsigned char		result;

	result = 0;
	while (SDL_PollEvent(&event))
	{
		key_code = event.key.keysym.sym;
		if (event.type == SDL_QUIT || key_code == SDLK_ESCAPE)
			flags->is_rtv1_over = true;
		result += keyboard_evens(event.type, key_code, flags);
	}
	if (result)
		keyboard_handle(cam,flags, cam->move_speed, cam->rotate_speed);
	return (result);
}
