/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 15:26:35 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		keyboard_handle(t_cam *restrict cam, t_speed *restrict sp,
								t_flags *restrict f)
{
	cam->pos.x += f->move.x * sp->move;
	cam->pos.y += f->move.y * sp->move;
	cam->pos.z += f->move.z * sp->move;
	cam->rotate_angle.x += f->rotate.x * sp->rotate;
	cam->rotate_angle.y += f->rotate.y * sp->rotate;
	cam->rotate_angle.z += f->rotate.z * sp->rotate;
}

static bool		keyboard_evens(Uint32 event_type, SDL_Keycode k,
								t_flags *restrict f)
{
	if (event_type == SDL_KEYDOWN
	&& (((k == SDLK_a || k == SDLK_d) && (f->move.x = k == SDLK_a ? NEG : POS))
	|| ((k == SDLK_w || k == SDLK_s) && (f->move.y = k == SDLK_w ? NEG : POS))
	|| ((k == SDLK_q || k == SDLK_e) && (f->move.z = k == SDLK_q ? NEG : POS))
	|| ((k == SDLK_t || k == SDLK_g) && (f->rotate.x = k == SDLK_t ? NEG : POS))
	|| ((k == SDLK_y || k == SDLK_h) && (f->rotate.y = k == SDLK_y ? NEG : POS))
	|| ((k == SDLK_u || k == SDLK_j) && (f->rotate.z = k == SDLK_u ? NEG : POS))
	))
		return (true);
//	if (event_type == SDL_KEYUP
//	&& (((k == SDLK_a || k == SDLK_d) && (f->move.x = NON))
//	|| ((k == SDLK_w || k == SDLK_s) && (f->move.y = NON))
//	|| ((k == SDLK_q || k == SDLK_e) && (f->move.z = NON))
//	|| ((k == SDLK_t || k == SDLK_g) && (f->rotate.x = NON))
//	|| ((k == SDLK_y || k == SDLK_h) && (f->rotate.y = NON))
//	|| ((k == SDLK_u || k == SDLK_j) && (f->rotate.z = NON))))
//		return (false);
	return (false);
}

_Bool			event_handler(t_cam *cam, t_flags *flags)
{
	SDL_Event		event;
	SDL_Keycode		key_code;
	unsigned char	result;

	result = 0;
	ft_bzero(flags, sizeof(t_flags));
	while (SDL_PollEvent(&event))
	{
		key_code = event.key.keysym.sym;
		if (event.type == SDL_QUIT || key_code == SDLK_ESCAPE)
			flags->is_rtv1_over = true;
		result += keyboard_evens(event.type, key_code, flags);
	}
	keyboard_handle(cam, &cam->speed, flags);
	return (result);
}
