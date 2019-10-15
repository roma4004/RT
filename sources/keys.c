/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/15 17:30:45 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void		keyboard_handle(t_env *env, t_flags *restrict f,
								double move_speed, double rotate_speed)
{
	t_dvec3		rot;
	int			obj_cnt;
	int			i;
	void		(*uni_move)(t_env *, t_dvec3 *, double);
	void		(*uni_rotate)(t_env *, t_dvec3);

	i = -1;
	obj_cnt = 0;
	while (++i < env->uni_arr_len)
		if (env->uni_arr[i].is_selected)
			obj_cnt++;
	uni_move = (obj_cnt) ? move_objects : move_camera;
	uni_rotate = (obj_cnt) ? rotate_objects : rotate_camera;
	if (f->move.x)
		uni_move(env, &env->origin_dir_x, (double)f->move.x * move_speed);
	if (f->move.y)
		uni_move(env, &env->origin_dir_y, (double)f->move.y * move_speed);
	if (f->move.z)
		uni_move(env, &env->origin_dir_z, (double)f->move.z * move_speed);
	rot = (t_dvec3){f->rotate.x * rotate_speed,
					f->rotate.y * rotate_speed,
					f->rotate.z * rotate_speed};
	uni_rotate(env, rot);
	if (f->is_reset)
		reset(env, &env->cam, obj_cnt);
}

static _Bool	is_key_recognized(t_flags *f, SDL_Keycode k)
{
	if ((k == SDLK_CAPSLOCK && ft_switch(&f->is_in_select_mod))
	|| (k == SDLK_r && ft_switch(&f->is_reset)))
		return (true);
	return (false);
}

static _Bool	keyboard_events(Uint32 event_type, SDL_Keycode k,
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
								|| is_z_rotate_up(k, &f->rotate)
								|| is_key_recognized(f, k)
								))
		return (true);
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

	if (event->type == SDL_MOUSEBUTTONUP)
	{
		double		dist;
		t_ray		ray;

		if (env->flags.is_in_select_mod && event->button.button == SDL_BUTTON_LEFT)
		{

			printf("event->motion.x = %d, event->motion.y = %d \n", event->motion.x, event->motion.y);
			ray = (t_ray){.t_min = cam->t_min,
				.t_max = cam->t_max,
				.pos = cam->pos,
				.dept_limit = cam->reflective_dept
			};
			ray.dir = convert_to_viewport(event->motion.x - env->cam.canvas.half.x,
										-event->motion.y + env->cam.canvas.half.y - 1.0,
										env->cam.canvas.rate);
			printf("ray.dir = %f, %f, %f \n", ray.dir.x, ray.dir.y, ray.dir.z);
			rotate_vec(&ray.dir, &env->cam.rotate_angle);

			send_selected_ray(env, &ray, &env->selected_obj, (double)MAXFLOAT);
			if (env->selected_obj)
				env->selected_obj->is_selected =
					(env->selected_obj->is_selected) ? false : true;
//				obj->diffuse_color = (t_dvec3){255,255,255,0};
			draw_scene(env, env->threads);
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
//		reset(SDLK_r, cam);
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
		result += mouse_events(env, &event, cam);
	}
	if (result)
		keyboard_handle(env, flags, cam->move_speed, cam->rotate_speed);
	return (result);
}
