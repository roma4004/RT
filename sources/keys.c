/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/24 15:21:36 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		keyboard_evens(t_cam *cam, Uint32 etype, SDL_Keycode k, t_flags *f)
{//need to separate to another pthread
	bool		*flag;
	bool		value;

	if (((etype == SDL_KEYDOWN && (value = true))
		|| (etype == SDL_KEYUP && !(value = false)))
	&& ((k == SDLK_ESCAPE	&& (flag = &f->is_rtv1_over))

		|| (k == SDLK_a		&& (flag = &f->is_move_x_less))
		|| (k == SDLK_d		&& (flag = &f->is_move_x_more))

		|| (k == SDLK_w		&& (flag = &f->is_move_y_more))
		|| (k == SDLK_s		&& (flag = &f->is_move_y_less))

		|| (k == SDLK_e		&& (flag = &f->is_move_z_more))
		|| (k == SDLK_q		&& (flag = &f->is_move_z_less))

		|| (k == SDLK_t		&& (flag = &f->is_rotate_x_more))
		|| (k == SDLK_g		&& (flag = &f->is_rotate_x_less))

		|| (k == SDLK_y		&& (flag = &f->is_rotate_y_more))
		|| (k == SDLK_h		&& (flag = &f->is_rotate_y_less))

		|| (k == SDLK_u		&& (flag = &f->is_rotate_z_more))
		|| (k == SDLK_j		&& (flag = &f->is_rotate_z_less))
		)
	)
		*flag = value;
//	*flag ^= *flag;
//	*flag &= 0;
//	*flag |= 1;
//	if (etype == SDL_KEYUP && k == SDLK_2 && f->mode++ >= 2)
//		f->mode = COLOR_TEX;
//	if (etype == SDL_KEYUP && k == SDLK_1)
//		f->is_compass_texture = (f->is_compass_texture) ? false : true;


	if (etype == SDL_KEYDOWN)
	{
		if (k == SDLK_a)
			cam->pos.x += 0.1;
		if (k == SDLK_d)
			cam->pos.x -= 0.1;


		if (k == SDLK_w)
			cam->pos.y -= 0.1;
		if (k == SDLK_s)
			cam->pos.y += 0.1;


		if (k == SDLK_e)
			cam->pos.z += 0.1;
		if (k == SDLK_q)
			cam->pos.z -= 0.1;


		if (k == SDLK_t)
			cam->rotate_angle.x++;
		if (k == SDLK_g)
			cam->rotate_angle.x--;

		if (k == SDLK_y)
			cam->rotate_angle.y++;
		if (k == SDLK_h)
			cam->rotate_angle.y--;

		if (k == SDLK_u)
			cam->rotate_angle.z++;
		if (k == SDLK_j)
			cam->rotate_angle.z--;
	}




}

void			event_handler(t_env *env, t_cam *cam, t_flags *flags)
{
	SDL_Event	event;
	SDL_Keycode	k;
	int			sign;

	while (SDL_PollEvent(&event))
	{
		k = event.key.keysym.sym;
		if (event.type == SDL_QUIT
		|| (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
			flags->is_rtv1_over = true;
//		mouse_events(&event, cam);
		keyboard_evens(cam, event.type, event.key.keysym.sym, flags);

	}
}
