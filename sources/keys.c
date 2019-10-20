/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/20 15:46:33 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		keyboard_handle(t_env *env, t_flags *restrict f,
					double move_speed, double rotate_speed)
{
	t_dvec3		rot;
	size_t		obj_cnt;
	void		(*uni_move)(t_env *, const t_dvec3 *, double);
	void		(*uni_rotate)(t_env *, t_dvec3);

	count_selected_obj(&obj_cnt, env->uni_arr, env->uni_arr_len);
	uni_move = (obj_cnt) ? move_objects : move_camera;
	uni_move(env, &env->cam.origin_dir_x, (double)f->move.x * move_speed);
	uni_move(env, &env->cam.origin_dir_y, (double)f->move.y * move_speed);
	uni_move(env, &env->cam.origin_dir_z, (double)f->move.z * move_speed);
	rot = (t_dvec3){
		.x = f->rotate.x * rotate_speed,
		.y = f->rotate.y * rotate_speed,
		.z = f->rotate.z * rotate_speed};
	uni_rotate = (obj_cnt) ? rotate_objects : rotate_camera;
	uni_rotate(env, rot);
	swith_handle(env, &env->flags, obj_cnt);
}

static _Bool	is_key_recognized(t_flags *f, SDL_Keycode k)
{
	if ((k == SDLK_CAPSLOCK && ft_switch(&f->is_in_select_mod))
	|| (k == SDLK_r && ft_switch(&f->is_reset))
	|| (k == SDLK_1 && ft_switch(&f->is_sepia))
	|| (k == SDLK_2 && ft_switch(&f->is_grayscale))
	|| (k == SDLK_4 && ft_switch(&f->is_screenshot)))
		return (true);
	return (false);
}

static _Bool	keyboard_events(Uint32 event_type, SDL_Keycode k,
								t_flags *restrict f)
{
	if (event_type == SDL_KEYDOWN && (is_x_move_down(&f->move, k)
									|| is_y_move_down(&f->move, k)
									|| is_z_move_down(&f->move, k)
									|| is_x_rotate_down(&f->rotate, k)
									|| is_y_rotate_down(&f->rotate, k)
									|| is_z_rotate_down(&f->rotate, k)))
		return (true);
	if (event_type == SDL_KEYUP && (is_x_move_up(&f->move, k)
								|| is_y_move_up(&f->move, k)
								|| is_z_move_up(&f->move, k)
								|| is_x_rotate_up(&f->rotate, k)
								|| is_y_rotate_up(&f->rotate, k)
								|| is_z_rotate_up(&f->rotate, k)
								|| is_key_recognized(f, k)))
		return (true);
	return (false);
}

static _Bool	mouse_events(t_env *env, SDL_Event *event)
{
	if (event->type == SDL_MOUSEMOTION)
	{
		mouse_move(env, event);
		mouse_rotate(env, event);
		mouse_rotate_z(env, event);
	}
	if (event->type == SDL_MOUSEBUTTONUP)
	{
		select_mod(env, event, &env->cam);
	}
	if (event->type == SDL_MOUSEWHEEL)
	{
		if (event->wheel.y > 0)
			env->flags.move.z = event->wheel.y;
		else if (event->wheel.y < 0)
			env->flags.move.z = event->wheel.y;
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
			env->flags.is_rtv1_over = true;
		result += keyboard_events(event.type, key_code, flags);
		result += mouse_events(env, &event);
	}
	if (result)
		keyboard_handle(env, flags, cam->move_speed, cam->rotate_speed);
	return (result);
}
