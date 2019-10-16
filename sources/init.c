/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 17:23:17 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/06 13:50:32 by dromanic         ###   ########.fr       */
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
	cam->canvas.half = (t_dvec){ WIN_WIDTH / 2.0, WIN_HEIGHT / 2.0};
	cam->canvas.rate = (double)WIN_WIDTH / WIN_HEIGHT;
	cam->move_speed = 1.2;
	cam->rotate_speed = 2.9;
	cam->t_min = 0;
	cam->t_max = (double)MAXFLOAT;
	cam->pos = (t_dvec3){0.0, 0.0, -42.0, 0.0};
	cam->rotate_angle = (t_dvec3){0.0, 0.0, 0.0, 0.0};
	cam->reflective_dept = 5;
	cam->epsilon = 0.00001;
}

static t_env		*env_def_val(t_env *env)
{
	if (!env)
		return (NULL);
	init_cam(&env->cam);
	env->bg_color = (t_dvec3){255.0, 255.0, 255.0, 0.0};
	env->threads = 1;
	errno = 0;
	return (env);
}

t_env				*init_sdl2(t_env *env)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)
	|| !(env->window = SDL_CreateWindow(WIN_NAME, SDL_WINDOWPOS_UNDEFINED,
										SDL_WINDOWPOS_UNDEFINED,
										WIN_WIDTH, WIN_HEIGHT,SDL_WINDOW_SHOWN))
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

t_env				*init_env(void)
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
