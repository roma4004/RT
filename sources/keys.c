/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/06 20:18:46 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static _Bool	is_cam_reset(SDL_Keycode k, t_cam *restrict cam)
{
	if (k == SDLK_r)
	{
		cam->pos = cam->parse_pos;
		cam->rotate_angle = cam->parse_rotate_angle;
		return (true);
	}
	return (false);
}

static void		keyboard_handle(t_env *env, t_cam *restrict cam,
								t_flags *restrict f,
								double move_speed, double rotate_speed)
{
	t_dvec3		offset;

	if (f->move.x)
	{
		vec3_mul_double(&offset, &env->origin_dir_x,
						(double)f->move.x * move_speed);
		vec3_add_vec3(&cam->pos, &cam->pos, &offset);
	}
	if (f->move.y)
	{
		vec3_mul_double(&offset, &env->origin_dir_y,
						(double)f->move.y * move_speed);
		vec3_add_vec3(&cam->pos, &cam->pos, &offset);
	}
	if (f->move.z)
	{
		vec3_mul_double(&offset, &env->origin_dir_z,
						(double)f->move.z * move_speed);
		vec3_add_vec3(&cam->pos, &cam->pos, &offset);
	}
//	cam->pos.x += f->move.x * move_speed;
//	cam->pos.y += f->move.y * move_speed;
//	cam->pos.z += f->move.z * move_speed;
	cam->rotate_angle.x += f->rotate.x * rotate_speed;
	cam->rotate_angle.y += f->rotate.y * rotate_speed;
	cam->rotate_angle.z += f->rotate.z * rotate_speed;
}

static _Bool	keyboard_evens(t_cam *cam,Uint32 event_type, SDL_Keycode k,
								t_flags *restrict f)
{
	if (event_type == SDL_KEYDOWN && (is_x_move_down(k, &f->move)
									|| is_y_move_down(k, &f->move)
									|| is_z_move_down(k, &f->move)
									|| is_x_rotate_down(k, &f->rotate)
									|| is_y_rotate_down(k, &f->rotate)
									|| is_z_rotate_down(k, &f->rotate)
									|| is_cam_reset(k,cam)))
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

static _Bool		mouse_events(t_env *env, SDL_Event *event, t_cam *cam)
{
	if (event->type == SDL_MOUSEMOTION)
	{
		if (event->button.button == SDL_BUTTON_X1)
		{
			if (event->motion.xrel < -1)
				env->flags.rotate.x = NEG;
			else if (event->motion.xrel > 1)
				env->flags.rotate.x = POS;			//			printf("event->motion.yrel = %d\n", event->motion.yrel);
			if (event->motion.yrel < -1)
				env->flags.rotate.y = POS;
			else if (event->motion.yrel > 1)
				env->flags.rotate.y = NEG;
			return (true);
		}
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			if (event->motion.xrel < -1)
				env->flags.move.x = POS;
			else if (event->motion.xrel > 1)
				env->flags.move.x = NEG;			//	printf("event->motion.yrel = %d\n", event->motion.yrel);
			if (event->motion.yrel < -1)
				env->flags.move.y = POS;
			else if (event->motion.yrel > 1)
				env->flags.move.y = NEG;
			return (true);
		}
		if (event->button.button == SDL_BUTTON_MIDDLE)
		{
			if (event->motion.xrel < -2)
				env->flags.rotate.z = NEG;
			else if (event->motion.xrel > 2)
				env->flags.rotate.z = POS;
			return (true);
		}
	}
	if (event->type == SDL_MOUSEWHEEL)
	{
		if (event->wheel.y > 0)
			env->flags.move.z = event->wheel.y;
		else if (event->wheel.y < 0)
			env->flags.move.z = event->wheel.y;
		return (true);
	}
	if (event->type == SDL_MOUSEBUTTONUP
	&& event->button.button == SDL_BUTTON_MIDDLE)
	{
//		is_cam_reset(SDLK_r, cam);
		return (true);
	}
	return (false);
}

_Bool			event_handler(t_env *env, t_cam *cam, t_flags *flags)
{
	SDL_Event			event;
	SDL_Keycode			key_code;
	unsigned char		result;

	result = 0;
	env->flags.move.x = NON;
	env->flags.move.y = NON;
	env->flags.move.z = NON;
	env->flags.rotate.x = NON;
	env->flags.rotate.y = NON;
	env->flags.rotate.z = NON;
	while (SDL_PollEvent(&event))
	{
		key_code = event.key.keysym.sym;
		if (event.type == SDL_QUIT
		|| (event.type == SDL_KEYDOWN && key_code == SDLK_ESCAPE))
			env->is_rtv1_over = true;
		result += keyboard_evens(cam, event.type, key_code, flags);
		result += mouse_events(env, &event, cam);
	}
	if (result)
		keyboard_handle(env, cam, flags, cam->move_speed, cam->rotate_speed);
	return (result);
}
