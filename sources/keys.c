/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/27 10:17:10 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		keyboard_handle(t_cam *cam, t_flags *f)
{
	if (f->is_move_x_more)
		cam->pos.x += cam->move_speed;
	if (f->is_move_x_less)
		cam->pos.x -= cam->move_speed;
	if (f->is_move_y_more)
		cam->pos.y += cam->move_speed;
	if (f->is_move_y_less)
		cam->pos.y -= cam->move_speed;
	if (f->is_move_z_more)
		cam->pos.z += cam->move_speed;
	if (f->is_move_z_less)
		cam->pos.z -= cam->move_speed;
	if (f->is_rotate_x_more)
		cam->rotate_angle.x += cam->rotate_speed;
	if (f->is_rotate_x_less)
		cam->rotate_angle.x -= cam->rotate_speed;
	if (f->is_rotate_y_more)
		cam->rotate_angle.y += cam->rotate_speed;
	if (f->is_rotate_y_less)
		cam->rotate_angle.y -= cam->rotate_speed;
	if (f->is_rotate_z_more)
		cam->rotate_angle.z += cam->rotate_speed;
	if (f->is_rotate_z_less)
		cam->rotate_angle.z -= cam->rotate_speed;
}
static bool		keyboard_evens(Uint32 etype, SDL_Keycode key_code,
								t_flags *flags)
{//need to separate to another pthread

	bool		*flag;
	bool		value;

	value = false;
	if (((etype == SDL_KEYDOWN && (value = true))
		|| (etype == SDL_KEYUP && !(value = false)))
	&& ((key_code == SDLK_a && (flag = &flags->is_move_x_less))
		|| (key_code == SDLK_d && (flag = &flags->is_move_x_more))
		|| (key_code == SDLK_w && (flag = &flags->is_move_y_more))
		|| (key_code == SDLK_s && (flag = &flags->is_move_y_less))
		|| (key_code == SDLK_e && (flag = &flags->is_move_z_more))
		|| (key_code == SDLK_q && (flag = &flags->is_move_z_less))
		|| (key_code == SDLK_t && (flag = &flags->is_rotate_x_more))
		|| (key_code == SDLK_g && (flag = &flags->is_rotate_x_less))
		|| (key_code == SDLK_y && (flag = &flags->is_rotate_y_more))
		|| (key_code == SDLK_h && (flag = &flags->is_rotate_y_less))
		|| (key_code == SDLK_u && (flag = &flags->is_rotate_z_more))
		|| (key_code == SDLK_j && (flag = &flags->is_rotate_z_less))))
		*flag = value;
	return (value);
//	*flag ^= *flag;
//	*flag &= 0;
//	*flag |= 1;
//	if (etype == SDL_KEYUP && key_code == SDLK_2 && flags->mode++ >= 2)
//		flags->mode = COLOR_TEX;
//	if (etype == SDL_KEYUP && key_code == SDLK_1)
//		flags->is_compass_texture = (flags->is_compass_texture) ? false : true;
}

bool			event_handler(t_cam *cam, t_flags *flags)
{
	SDL_Event		event;
	SDL_Keycode		key_code;
	unsigned char	result;

	result = 0;
	while (SDL_PollEvent(&event))
	{
		key_code = event.key.keysym.sym;
		if (event.type == SDL_QUIT || key_code == SDLK_ESCAPE)
			flags->is_rtv1_over = true;
//		mouse_events(&event, cam);
		result += keyboard_evens(event.type, key_code, flags);
	}
	keyboard_handle(cam, flags);
	return (result);
}
