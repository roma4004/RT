/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/07 18:39:43 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		move(t_cam *cam, Uint32 **map, double dir_x, double dir_y)
{
	Uint32		x;
	Uint32		y;

	x = (Uint32)(cam->pos.x + dir_x * cam->move_speed * cam->min_wall_dist);
	y = (Uint32)(cam->pos.y + dir_y * cam->move_speed * cam->min_wall_dist);
	if (!map[x][y])
	{
		cam->pos.x += dir_x * cam->move_speed;
		cam->pos.y += dir_y * cam->move_speed;
	}
}

static void		rotate_horiz(t_cam *cam, double angle,
								double dir_x, double plane_x)
{
	cam->dir.x = cam->dir.x * cos(angle) - cam->dir.y * sin(angle);
	cam->dir.y = dir_x * sin(angle) + cam->dir.y * cos(angle);
	cam->plane.x = cam->plane.x * cos(angle) - cam->plane.y * sin(angle);
	cam->plane.y = plane_x * sin(angle) + cam->plane.y * cos(angle);
}

static void		mouse_events(SDL_Event *event, t_cam *cam)
{
	if (event->type == SDL_MOUSEMOTION)
	{
		if (event->motion.xrel < 0)
			rotate_horiz(cam, -event->motion.xrel / 1000.0,
						cam->dir.x, cam->plane.x);
		else if (event->motion.xrel > 0)
			rotate_horiz(cam, event->motion.xrel / -1000.0,
						cam->dir.x, cam->plane.x);
		else
			event->motion.xrel = 0;
	}
	if (event->type == SDL_MOUSEWHEEL)
	{
		if (event->wheel.y > 0 && cam->zoom < 10)
			cam->zoom += 0.1;
		else if (event->wheel.y < 0 && cam->zoom > 1)
			cam->zoom -= 0.1;
	}
	if (event->type == SDL_MOUSEBUTTONUP
	&& event->button.button == SDL_BUTTON_MIDDLE)
		cam->zoom = 1;
}

static void		keyboard_evens(Uint32 etype, SDL_Keycode k, t_flags *f)
{//need to separate to another pthread
	bool		*flag;
	bool		value;

	if (((etype == SDL_KEYDOWN && (value = true))
		|| (etype == SDL_KEYUP && !(value = false)))
	&& (	(k == SDLK_ESCAPE					&& (flag = &f->is_game_over))
//		|| ((k == SDLK_q || k == SDLK_LEFT)		&& (flag = &f->is_rotate_left))
//		|| ((k == SDLK_e || k == SDLK_RIGHT)	&& (flag = &f->is_rotate_right))
		|| ((k == SDLK_w || k == SDLK_UP)		&& (flag = &f->is_move_forward))
		|| ((k == SDLK_s || k == SDLK_DOWN)		&& (flag = &f->is_move_backward))
		|| ((k == SDLK_PAGEUP || k == SDLK_e)	&& (flag = &f->is_move_z_forward))
		|| ((k == SDLK_PAGEDOWN || k == SDLK_q)	&& (flag = &f->is_move_z_backward))
		|| (k == SDLK_a							&& (flag = &f->is_strafe_left))
		|| (k == SDLK_d							&& (flag = &f->is_strafe_right))))
		*flag = value;
//	*flag ^= *flag;
//	*flag &= 0;
//	*flag |= 1;
	if (etype == SDL_KEYUP && k == SDLK_2 && f->mode++ >= 2)
		f->mode = COLOR_TEX;
	if (etype == SDL_KEYUP && k == SDLK_1)
		f->is_compass_texture = (f->is_compass_texture) ? false : true;
}

void			event_handler(t_env *env, t_cam *cam, t_flags *flags)
{
	int			sign;

	while (SDL_PollEvent(&env->event))
	{
		if (env->event.type == SDL_QUIT)
			flags->is_game_over = true;
		mouse_events(&env->event, cam);
		keyboard_evens(env->event.type, env->event.key.keysym.sym, flags);
	}
	if (flags->is_strafe_left)
		env->cam.pos.x += 0.1;
	if (flags->is_strafe_right)
		env->cam.pos.x -= 0.1;

	if (flags->is_move_forward)
		env->cam.pos.y -= 0.1;
	if (flags->is_move_backward)
		env->cam.pos.y += 0.1;

	if (flags->is_move_z_forward)
		env->cam.pos.z += 0.1;
	if (flags->is_move_z_backward)
		env->cam.pos.z -= 0.1;



	if ((flags->is_rotate_left && (sign = 1))
	|| (flags->is_rotate_right && (sign = -1)))
		rotate_horiz(cam, cam->rotate_speed * sign, cam->dir.x, cam->plane.x);
}
