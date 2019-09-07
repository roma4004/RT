/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 17:23:17 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/07 20:17:49 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static SDL_Surface		*load_surface(t_env *env, char *path_name)
{
	SDL_Surface			*new_srf;
	SDL_Surface			*convert_srf;
	SDL_PixelFormat		px_format;

	if (!env || !path_name)
		return (NULL);
	ft_bzero(&px_format, sizeof(SDL_PixelFormat));
	px_format.format = SDL_PIXELFORMAT_ARGB8888;
	px_format.BytesPerPixel = sizeof(Uint32);
	px_format.BitsPerPixel = (Uint8)(px_format.BytesPerPixel << 3u);
	if (!(new_srf = IMG_Load(path_name))
	|| !(convert_srf = SDL_ConvertSurface(new_srf, &px_format, 0)))
	{
		ft_putstr(SDL_GetError());
		ft_putchar('\n');
		env->err_id = INVALID_RESOURCE;
		return (NULL);
	}
	if (new_srf)
		SDL_FreeSurface(new_srf);
	return (convert_srf);
}

static void			init_cam(t_cam *cam)
{
	ft_bzero(cam, sizeof(t_cam));
	cam->dir.x = -1;
	cam->plane.y = 0.66;
	cam->center = (t_ui32pt){ WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };
	cam->zoom = 1;
	cam->min_wall_dist = 8.5;
}

static void				init_img_tex(t_env *env, SDL_Surface **img_tex)
{
	img_tex[0] = load_surface(env, "resources/textures/eagle.png");
	img_tex[1] = load_surface(env, "resources/textures/red_brick.png");
	img_tex[2] = load_surface(env, "resources/textures/purple_stone.png");
	img_tex[3] = load_surface(env, "resources/textures/grey_stone.png");
	img_tex[4] = load_surface(env, "resources/textures/blue_stone.png");
	img_tex[5] = load_surface(env, "resources/textures/mossy.png");
	img_tex[6] = load_surface(env, "resources/textures/wood.png");
	img_tex[7] = load_surface(env, "resources/textures/color_stone.png");
}

static t_env			*env_def_val(t_env *env)
{
	if (!env)
		return (NULL);
	env->err_id = 0;
	ft_bzero(&env->flags, sizeof(t_flags));
	env->flags.is_compass_texture = 1;
	env->flags.mode = IMAGE_TEX;
	init_cam(&env->cam);
	ft_bzero(&env->fps, sizeof(t_fps));
	env->fps.frame_limit_second = 1000 / FRAME_LIMIT;
	env->bytes_per_pixel = sizeof(Uint32);
	env->bits_per_pixel = (Uint8)(env->bytes_per_pixel << 3u);
//	generate_texture(env);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	init_img_tex(env, env->img_tex);
	if (env->err_id)
		return (NULL);
	env->txt.color = (SDL_Color){ 255, 255, 255, 0 };
	env->txt.width = 0;
	env->txt.height = 0;
	env->threads = 4;






	env->cam.t_min = 1;
	env->cam.t_max = MAXFLOAT;
	env->canvas_half = (t_fvec){ WIN_WIDTH / 2., WIN_HEIGHT / 2.};
	env->cam.pos = (t_fvec3){ 0, 0, 0 };
///tmp init obj
	env->sphere_arr[0] = (t_sphr){(t_fvec3){0,-1, 3}, 1, (t_fvec3){255, 0, 0}};
	env->sphere_arr[1] = (t_sphr){(t_fvec3){2, 0, 4}, 1, (t_fvec3){0, 0, 255}};
	env->sphere_arr[2] = (t_sphr){(t_fvec3){-2, 0, 4}, 1, (t_fvec3){0, 255, 0}};
	env->sphere_arr[3] = (t_sphr){ (t_fvec3){0, -5000, 0}, 5000, (t_fvec3){255,
																		   255, 0}};
	env->light_arr[0] = (t_lght){LIGHT_AMBIENT, (t_fvec3){ 0, 0, 0 }, 0.2 };
	env->light_arr[1] = (t_lght){LIGHT_POINT, (t_fvec3){ 2, 1, 0 }, 0.6 };
	env->light_arr[2] = (t_lght){LIGHT_DIRECTIONAL, (t_fvec3){ 1, 4, 4 }, 0.2 };

	return (env);
}

t_env					*init_env(void)
{
	t_env	*env;

	if (!(env = (t_env *)malloc(sizeof(t_env)))
	|| SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)
	|| TTF_Init() || !IMG_Init(IMG_INIT_PNG)
	|| !(env->window = SDL_CreateWindow(WIN_NAME, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE))
	|| !(env->renderer = SDL_CreateRenderer(env->window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
	|| !(env->screen = SDL_CreateTexture(env->renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
		WIN_WIDTH, WIN_HEIGHT))
	|| !(env->state = SDL_GetKeyboardState(&env->state_arr_length))
	|| !(env->img_tex = malloc(sizeof(SDL_Surface *) * TEXTURES))
	|| !(env->txt.font = TTF_OpenFont(DEF_FONT, DEF_FONT_SIZE))
	|| (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	|| !(env->music = Mix_LoadMUS("resources/sounds/horst_wessel_lied.wav"))

	|| !(env->sphere_arr = (t_sphr *)malloc(sizeof(t_sphr) * SPHERE_CNT))
	|| !(env->light_arr = (t_lght *)malloc(sizeof(t_lght) * LIGHTS_CNT))
	|| !(env_def_val(env))

	)
	{

		write(1,"0",1);
//		quit_program(env);
		return (NULL);
	}
	return (env);
}
