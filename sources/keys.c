/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/10 19:08:39 by dromanic         ###   ########.fr       */
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



//	if ((flags->is_rotate_left && (sign = 1))
//	|| (flags->is_rotate_right && (sign = -1)))
//		rotate_horiz(cam, cam->rotate_speed * sign, cam->dir.x, cam->plane.x);
}
