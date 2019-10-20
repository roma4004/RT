/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 17:23:17 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 15:17:46 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		init_cam(t_cam *cam)
{
	cam->half = (t_dvec){ WIN_WIDTH / 2.0, WIN_HEIGHT / 2.0};
	cam->rate = (double)WIN_WIDTH / WIN_HEIGHT;
	cam->move_speed = 1.2;
	cam->rotate_speed = 2.9;
	cam->t_min = 0;
	cam->t_max = (double)MAXFLOAT;
	cam->pos = (t_dvec3){0.0, 0.0, -42.0, 0.0};
	cam->rotate_angle = (t_dvec3){0.0, 0.0, 0.0, 0.0};
	cam->reflective_dept = 5;
	cam->epsilon = 0.00001;
}

static t_env	*env_def_val(t_env *env)
{
	if (!env)
		return (NULL);
	init_cam(&env->cam);
	env->bg_color = (t_dvec3){255.0, 255.0, 255.0, 0.0};
	env->threads = 4;
	errno = 0;
	return (env);
}

t_env			*init_sdl2(t_env *env)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)
	|| !(env->window = SDL_CreateWindow(WIN_NAME,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN))
	|| !(env->renderer = SDL_CreateRenderer(env->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
	|| !(env->screen = SDL_CreateTexture(env->renderer,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
			WIN_WIDTH, WIN_HEIGHT)))
	{
		quit_program(env);
		return (NULL);
	}
	return (env);
}

t_env			*init_env(void)
{
	t_env	*env;

	if ((env = (t_env *)malloc(sizeof(t_env))))
	{
		ft_bzero(env, sizeof(t_env));
		env_def_val(env);
	}
	else
	{
		quit_program(env);
		return (NULL);
	}
	return (env);
}
