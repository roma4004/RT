/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/08/12 19:04:13 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void			quit_program(t_env *env)
{
	if (!env)
		return ;
	//need clear env
	if (env->screen)
		SDL_DestroyTexture(env->screen);
	env->screen = NULL;
	if (env->renderer)
		SDL_DestroyRenderer(env->renderer);
	env->renderer = NULL;
	if (env->window)
		SDL_DestroyWindow(env->window);
	env->window = NULL;
	if (env->surface)
		SDL_FreeSurface(env->surface);
	env->surface = NULL;
	if (env->music)
		Mix_CloseAudio();
	env->music = NULL;
	show_errors(env);
	SDL_Quit();
}

static void		clear_img_buff(t_env *env)
{
	Uint32	y;
	Uint32	x;

	if (!env)
		return ;
	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
			env->buff[y][x++] = 0x0;
		y++;
	}
}

static int		prepare_render(t_env *env, t_fps *fps)
{
	fps->pre_tick = fps->cur_tick;
	fps->cur_tick = SDL_GetTicks();
	fps->frame_time = (fps->cur_tick - fps->pre_tick) / 1000.0;
	fps->value = (u_char)(1.0 / fps->frame_time);
	env->cam.move_speed = fps->frame_time * 3.0;
	env->cam.rotate_speed = fps->frame_time * 2.0;
//	if ((fps->frame_limit_second) > fps->cur_tick - fps->pre_tick)
//		SDL_Delay(fps->frame_limit_second - (fps->cur_tick - fps->pre_tick));
	event_handler(env, &env->cam, &env->flags);
	clear_img_buff(env);
	return (1);
}

void			game_loop(t_env *env, int threads)
{
	int			id;
	t_pth_dt	data[threads];
	pthread_t	threads_arr[threads];

	if (!env)
		return ;
	while (!env->flags.is_game_over && prepare_render(env, &env->fps))
	{
		id = -1;
		while (++id < threads)
		{
			data[id].env = env;
			data[id].offset = id;
			pthread_create(&threads_arr[id], NULL, multi_raycasting, &data[id]);
		}
		id = -1;
		while (++id < threads)
			pthread_join(threads_arr[id], NULL);
		SDL_UpdateTexture(env->screen, NULL, env->buff, WIN_WIDTH << 2);
		SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);
		if (SHOW_FPS)
			render_interface(env, &env->fps, &env->txt);
		SDL_RenderPresent(env->renderer);
	}
}
void				vec3_add(t_fvec3 *restrict destination,
								const t_fvec3 *restrict first,
								const t_fvec3 *restrict second)
{
	destination->x = first->x + second->x;
	destination->y = first->y + second->y;
	destination->z = first->z + second->z;
}

void				vec3_sub(t_fvec3 *restrict destination,
								const t_fvec3 *restrict first,
								const t_fvec3 *restrict second)
{
	destination->x = first->x - second->x;
	destination->y = first->y - second->y;
	destination->z = first->z - second->z;
}

void				vec3_mul(t_fvec3 *restrict destination,
								const t_fvec3 *restrict first,
								const t_fvec3 *restrict second)
{
	destination->x = first->x * second->x;
	destination->y = first->y * second->y;
	destination->z = first->z * second->z;
}

void				vec3_div(t_fvec3 *restrict destination,
								const t_fvec3 *restrict first,
								const t_fvec3 *restrict second)
{
	destination->x = first->x * second->x;
	destination->y = first->y * second->y;
	destination->z = first->z * second->z;
}


//sounds https://www.deezer.com/track/441682922?utm_source=deezer&utm_content=track-441682922&utm_term=2309518464_1565535009&utm_medium=web


//recurcive or iterated spawn of reflected and refracted rays
//
// list->list of rays which saves spot, when ray spawn new two rays reflect
// and refract

//ray_max_bounced

//list->root_node
//list->root_pos
//list->root_dir
//list->reflect_pos
//list->reflect_dir
//list->refracted_pos
//list->refracted_dir


//obj->pos
//obj->rotate_state{x,y,z}
//obj->color
//obj->is_reflected
//obj->is_refracted
//obj->is_light_source
//obj->name;
//obj->type;
//obj->opacity;



float	fclamp(float arg, float minimum, float maximum)
{
	return (fminf(fmaxf(arg, minimum), maximum));
}
static uint8_t  double2byte_clamp(const double x)
{
	if (__builtin_isless(x, 0.0))
		return (0);
	else if (__builtin_isgreater(x, 1.0))
		return (255);
	else
		return ((uint8_t)(255.0 * x));
}

int				main(int argc, char **argv)
{
	t_env *env;

	env = init_env();


    int y;
    int x;
	t_fvec3 framebuffer[WIN_WIDTH * WIN_HEIGHT];


	while (!env->flags.is_game_over && prepare_render(env, &env->fps))
	{
		char *buf_ch;
		buf_ch = (char *)env->buff;
		clear_img_buff(env);
		y = 0;
//		while (y < WIN_HEIGHT)
//		{
//			x = 0;
//			while (x < WIN_WIDTH)
//			{
//				buf_ch[y * WIN_WIDTH + x] = (y / (float)(WIN_HEIGHT))
//						(x / (float)(WIN_WIDTH))
//						(y / (float)(WIN_HEIGHT);
//
//				x++;
//			}
//			y++;
//		}

		for (size_t j = 0; j < WIN_HEIGHT; j++) {
			for (size_t i = 0; i < WIN_WIDTH; i++) {
				framebuffer[i + j * WIN_WIDTH] =
				(t_fvec3){ j / (float)(WIN_HEIGHT), i / (float)(WIN_WIDTH), 0 };
			}
		}

		for (size_t i = 0; i < WIN_HEIGHT * WIN_WIDTH; ++i)
		{
			((uint32_t*)buf_ch)[i] =
					double2byte_clamp(framebuffer[i].y) << (1 * 8)
				  | double2byte_clamp(framebuffer[i].x) << (2 * 8)
				  | double2byte_clamp(framebuffer[i].z) << (3 * 8);
		}
		event_handler(env, &env->cam, &env->flags);

		SDL_UpdateTexture(env->screen, NULL, env->buff, WIN_WIDTH << 2u);
		SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);

		SDL_RenderPresent(env->renderer);
	}








//	if (argc == 2)
//	{
//		if ((env = init_env()) && parse_map(argv[1], env)
//		&& !Mix_PlayMusic(env->music, 1))
//			game_loop(env, env->threads);
//	}
//	else
//		ft_putstr("Usage: ./rtv1 map.rtv\n");
//	if (env)
//		quit_program(env);
	return (0);
}
