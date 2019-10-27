/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 17:23:17 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/27 17:33:43 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		init_cam(t_env *env, t_cam *cam)
{
	cam->half = (t_dvec){
		.x = (double)env->buff_width / 2.0,
		.y = (double)env->buff_height / 2.0};
	cam->rate = (double)env->buff_width / (double)env->buff_height;
	cam->move_speed = 1.2;
	cam->rotate_speed = 2.9;
	cam->t_max = (double)MAXFLOAT;
	cam->reflective_dept = REFLECTIVE_MAX_DEPT;
	cam->epsilon = 0.00001;
}

t_env			*init_sdl2(t_env *env)
{
	if (!(env->buff =
		(Uint32 *)malloc(sizeof(Uint32) * env->buff_height * env->buff_width))
	|| SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)
	|| !(env->window = SDL_CreateWindow(WIN_NAME,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			env->buff_width, env->buff_height, SDL_WINDOW_SHOWN))
	|| !(env->renderer = SDL_CreateRenderer(env->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
	|| !(env->screen = SDL_CreateTexture(env->renderer,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
			env->buff_width, env->buff_height)))
	{
		quit_program(env);
		return (NULL);
	}
	init_cam(env, &env->cam);
	return (env);
}

t_env			*init_env(void)
{
	t_env	*env;

	if ((env = (t_env *)malloc(sizeof(t_env))))
	{
		ft_bzero(env, sizeof(t_env));
		env->buff_width = 640;
		env->buff_height = 480;
		env->threads = 4;
		env->bg_color = (t_dvec3){0,0,0,0};
		errno = 0;
	}
	else
	{
		quit_program(env);
		return (NULL);
	}
	return (env);
}
