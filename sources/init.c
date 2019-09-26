/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 17:23:17 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/26 18:19:50 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void			init_cam(t_cam *cam)
{
	ft_bzero(cam, sizeof(t_cam));
	cam->t_min = 1;        //ray not cam
	cam->t_max = MAXFLOAT;//ray not cam
	cam->pos = (t_dvec3){ 0, 0, -10 }; //ray not cam, ray start position
}


static t_env			*env_def_val(t_env *env)
{
	if (!env)
		return (NULL);
	env->err_id = 0;
	ft_bzero(&env->flags, sizeof(t_flags));
	init_cam(&env->cam);
	env->bytes_per_pixel = sizeof(Uint32);
	env->bits_per_pixel = (Uint8)(env->bytes_per_pixel << 3u);
	if (env->err_id)
		return (NULL);
	env->threads = 4;
	env->canvas_half = (t_dvec){ WIN_WIDTH / 2., WIN_HEIGHT / 2.};
///tmp init obj
	//                      unsigned, t_dvec3          , double       , t_dvec3     ,	t__mat mat  t_dvec3			, double			,	double;
	//                       type   ,  pos             , radius       , dir         ,	t__mat mat  diffuse_col	, specular			,	albedo;
	env->uni_arr[0] = (t_uni){SPHERE,(t_dvec3){4, -1, 3}, 	1.0   ,	(t_dvec3){0},	(t_mat){(t_dvec3){0, 0, 0},	10 			/*, 1.0*/}};//red
	env->uni_arr[1] = (t_uni){SPHERE,(t_dvec3){2, 4, 4},	1.0   ,	(t_dvec3){0},	(t_mat){(t_dvec3){0, 0, 255},	100 			/*, 0.5*/}}; //blue
	env->uni_arr[2] = (t_uni){SPHERE,(t_dvec3){-2, 4, 4},	1.0   ,	(t_dvec3){0},	(t_mat){(t_dvec3){0, 255, 0},	10  			/*, 0.9*/}};
	env->uni_arr[3] = (t_uni){SPHERE,(t_dvec3){1, -1, 4},	1.0,	(t_dvec3){0},	(t_mat){(t_dvec3){255, 255, 0},	1000			/*, 0.1*/}};
	env->uni_arr[4] = (t_uni){PLANE,(t_dvec3){5, 0, 0},		0.0,	(t_dvec3){0,1,0},	(t_mat){(t_dvec3){0,255,0},		1000			/*, 0.1*/}};
	env->uni_arr[5] = (t_uni){CONE,(t_dvec3){0, 0, 0},	15.0,	(t_dvec3){1,1,0},	(t_mat){(t_dvec3){255,0,255},		1000			/*, 0.1*/}};



	env->uni_arr[6] = (t_uni){CONE,(t_dvec3){0, 0, 0},	1.0,	(t_dvec3){0,1,0},	(t_mat){(t_dvec3){255,0,255},		1000			/*, 0.1*/}};


	env->light_arr[0] = (t_lght){AMBIENT,		(t_dvec3){ 0, 0, 0 }, 0.10, (t_dvec3){255, 255, 255} };
	env->light_arr[1] = (t_lght){POINT,			(t_dvec3){ 2, 1, 0 }, 0.6, (t_dvec3){255, 255, 255} };
	env->light_arr[2] = (t_lght){DIRECTIONAL,	(t_dvec3){ 1, 4, 4 }, 0.4, (t_dvec3){255, 255, 255} };

	env->bg_color = (t_dvec3){255, 255, 255};
	env->epsilon = 0.001;
	return (env);
}

t_env					*init_env(void)
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
	|| !(env->uni_arr = (t_uni *)malloc(sizeof(t_uni) * (SPHERE_CNT + PLANE_CNT + CYLINDER_CNT)))
	|| !(env->light_arr = (t_lght *)malloc(sizeof(t_lght) * LIGHTS_CNT))
	|| !(env_def_val(env)))
	{
//		quit_program(env);
		return (NULL);
	}
	return (env);
}
