/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 17:23:17 by dromanic          #+#    #+#             */
/*   Updated: 2020/01/05 21:10:01 by dromanic         ###   ########.fr       */
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
	cam->t_max = INFINITY;
	cam->reflective_dept = REFLECTIVE_MAX_DEPT;
	cam->epsilon = 0.00001;
}

t_env			*init_sdl2(t_env *env)
{
	if (!(env->buff =
		(Uint32 *)malloc(sizeof(Uint32) * env->buff_height * env->buff_width))
	|| SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)
	|| TTF_Init()
	|| !(env->window = SDL_CreateWindow(WIN_NAME,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			env->buff_width, env->buff_height, SDL_WINDOW_SHOWN))
	|| !(env->renderer = SDL_CreateRenderer(env->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
	|| !(env->screen = SDL_CreateTexture(env->renderer,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
			env->buff_width, env->buff_height))
	|| !(env->font =
		TTF_OpenFont("resources/fonts/Roboto/Roboto-Black.ttf", 28))
	|| !(env->tex_arr =
		(SDL_Surface **)malloc(sizeof(SDL_Surface *) * env->tex_arr_len))
	|| !init_img_tex(env, env->tex_arr))
		return (NULL);
	init_cam(env, &env->cam);
	return (env);
}

//	pthread_cond_t buffer_obsolete; //todo: add sleep until condition
//	int pthread_cond_broadcast(pthread_cond_t *cv);
//	https://www.opennet.ru/docs/RUS/linux_parallel/node75.html
void			init_thread(t_env *env, size_t threads)
{
	size_t		id;

	if (!env || !(env->data = (t_pth_dt *)malloc(threads * sizeof(t_pth_dt))))
		return ;
	if (!(env->threads_arr = (pthread_t *)malloc(threads * sizeof(pthread_t))))
	{
		free(env->data);
		return ;
	}
	id = UINT64_MAX;
	while (++id < threads)
	{
		env->thread_status[id] = false;
		env->data[id] = (t_pth_dt){.env = env, .id = id};
		pthread_create(&env->threads_arr[id], NULL,
			render_frame, &env->data[id]);
		pthread_detach(env->threads_arr[id]);
	}
}

t_env			*init_env(void)
{
	t_env	*env;

	if ((env = (t_env *)malloc(sizeof(t_env))))
	{
		ft_bzero(env, sizeof(t_env));
		env->buff_width = 640;
		env->buff_height = 480;
		env->threads = ft_get_processors_num();
		env->thread_status = (_Bool *)malloc(sizeof(_Bool) * env->threads);
		env->count_lock =
			(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * env->threads);
		pthread_cond_init(&env->buffer_obsolete, NULL);
		init_thread(env, env->threads);
		env->tex_arr_len = 4;
		env->bg_color = (t_dvec3){0, 0, 0, 0};
		errno = 0;
	}
	else
	{
		quit_program(env);
		return (NULL);
	}
	return (env);
}
