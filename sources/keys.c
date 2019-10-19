/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 15:22:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 19:05:58 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		keyboard_handle(t_env *env, t_flags *restrict f,
					double move_speed, double rotate_speed)
{
	t_dvec3		rot;
	size_t			obj_cnt;
	void		(*uni_move)
					(t_env *env, const t_dvec3 *move_dir, double move_speed);
	void		(*uni_rotate)
					(t_env *env, t_dvec3 rot);

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
								|| is_key_recognized(f, k)
								))
		return (true);
	return (false);
}

static _Bool	mouse_events(t_env *env, SDL_Event *event, t_cam *cam)
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

//			printf("event->motion.x = %d, event->motion.y = %d \n",
//				event->motion.x, event->motion.y);
			ray = (t_ray){.t_min = cam->t_min,
				.t_max = cam->t_max,
				.pos = cam->pos,
				.dept_limit = cam->reflective_dept
			};
			ray.dir = convert_to_viewport(event->motion.x - env->cam.half.x,
										-event->motion.y + env->cam.half.y - 1.0,
										env->cam.rate);
//			printf("ray.dir = %f, %f, %f \n", ray.dir.x, ray.dir.y, ray.dir.z);
			rotate_vec(&ray.dir, &env->cam.rotate_angle);

			send_selected_ray(env, &ray, &env->selected_obj, (double)MAXFLOAT);
			if (env->selected_obj)
				env->selected_obj->is_selected =
					(env->selected_obj->is_selected) ? false : true;
			select_caps_cylinder_cone(env);

//				obj->color = (t_dvec3){255,255,255,0};

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
