/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 17:23:17 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 20:12:50 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void			cnt_obj_type(t_env *env, t_list *lst)
{
	size_t		*lights;
	size_t		*other;
	t_list		*cur;
	size_t		type;

	lights = &env->light_arr_len;
	other = &env->uni_arr_len;
	cur = lst;
	while (cur)
	{
		if ((type = get_type(cur->content)) != UINT64_MAX)
		{
			if (type < 3u)
				(*lights)++;
			else if (type < OBJ_TYPE_MAX)
				(*other)++;
		}
		cur = cur->next;
	}
}

_Bool				init_obj_arr(t_env *env, t_list *lst)
{
	cnt_obj_type(env, lst);
	if ((env->uni_arr = (t_uni *)malloc(sizeof(t_uni) * env->uni_arr_len))
	&& (env->light_arr = (t_lght *)malloc(sizeof(t_lght) * env->light_arr_len)))
		return (true);
	else
	{
		if (env->uni_arr)
			ft_memdel((void **)&env->uni_arr);
		if (env->light_arr)
			ft_memdel((void **)&env->light_arr);
	}
	return (false);
}

static void			init_cam(t_cam *cam)
{
	ft_bzero(cam, sizeof(t_cam));
	cam->canvas.half = (t_dvec){ WIN_WIDTH / 2.0, WIN_HEIGHT / 2.0};
	cam->canvas.rate = (double)WIN_WIDTH / WIN_HEIGHT;
	cam->speed = (t_speed){.move = 1.2, .rotate = 2.9};
	cam->t_min = 0;
	cam->t_max = (double)MAXFLOAT;
	cam->pos = (t_dvec3){0.0, 0.0, -42.0, 0.0};
	cam->rotate_angle = (t_dvec3){0.0, 0.0, 0.0, 0.0};
}

static t_env		*env_def_val(t_env *env)
{
	if (!env)
		return (NULL);
	env->err_id = 0;
	ft_bzero(&env->flags, sizeof(t_flags));
	init_cam(&env->cam);
	if (env->err_id)
		return (NULL);
	env->bg_color = (t_dvec3){255.0, 255.0, 255.0, 0.0};
	env->epsilon = 0.00001;
	env->threads = 4;
	return (env);
}

t_env				*init_env(void)
{
	t_env	*env;

	if (!(env = (t_env *)malloc(sizeof(t_env)))
	|| SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)
	|| !(env->window = SDL_CreateWindow(WIN_NAME, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE))
	|| !(env->renderer = SDL_CreateRenderer(env->window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
	|| !(env->screen = SDL_CreateTexture(env->renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
		WIN_WIDTH, WIN_HEIGHT))
	|| (env->uni_arr = NULL)
	|| (env->light_arr = NULL)
	|| !(env_def_val(env)))
	{
		quit_program(env);
		return (NULL);
	}
	return (env);
}
