/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/08/13 18:59:35 by dromanic         ###   ########.fr       */
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
	if (x < 0.0)
		return (0);
	else if (x > 1.0)
		return (255);
	else
		return ((uint8_t)(255.0 * x));
}

//Sphere(const Vec3f &c, const float &r) : center(c), radius(r) {}
bool ray_intersect(const t_sphr *sphere, const t_fvec3 *orig,
					const t_fvec3 *dir, float *t0)
{
	float		thc;
	float		tca;
	t_fvec3		L;
	float		d2;
	t_fvec3		tmp;
	t_fvec3		tmp2;

	vec3_sub_vec3(&L, &sphere->center, orig);
	vec3_mul_vec3(&tmp, &L, dir);
	tca = vec3_to_float(&tmp);
	vec3_mul_vec3(&tmp, &L, &L);
	vec3_sub_float(&tmp2, &tmp, tca * tca);
	d2 = vec3_to_float(&tmp2);
	if (d2 > sphere->radius * sphere->radius)
		return false;
	thc = sqrtf(sphere->radius * sphere->radius - d2);
	*t0 = tca - thc;
	if (*t0 < 0)
		*t0 = tca + thc;
	if (*t0 < 0)
		return false;
	return true;
}

bool scene_intersect(const t_fvec3 *orig, const t_fvec3 *dir,
		const t_sphr *spheres, t_fvec3 *hit, t_fvec3 *N, t_mat *material)
{
	float		spheres_dist;
	float		dist_i;
	t_fvec3		tmp;

	spheres_dist = MAXFLOAT;//std::numeric_limits<float>::max();
	for (size_t i = 0; i < SPHERE_CNT; i++)
	{
		if (ray_intersect(&spheres[i], orig, dir, &dist_i)
		&& dist_i < spheres_dist)
		{
			spheres_dist = dist_i;
			vec3_mul_float(&tmp, dir, dist_i);
			vec3_add_vec3(hit, orig, &tmp);
			vec3_sub_vec3(&tmp, hit, &spheres[i].center);
			vec3_normalize(N, &tmp);
			*material = spheres[i].material;
		}
	}
	return (spheres_dist < 1000);
}

t_fvec3			cast_ray(const t_fvec3 *orig, const t_fvec3 *dir,
							const t_sphr *sphere_arr)
{
	t_fvec3		point;
	t_fvec3		N;
	t_mat		material;

	if (!scene_intersect(orig, dir, sphere_arr, &point, &N, &material))
	{
		return (t_fvec3){ 0.0, 0.0, 0.0 }; // background color
	}
	return (material.diffuse_color);
}

int				main(int argc, char **argv)
{
	t_env *env;

	env = init_env();

	t_fvec3	framebuffer[WIN_WIDTH * WIN_HEIGHT];
	t_sphr	*sphere_arr;
	t_mat	mat1;
	t_mat	mat2;
	t_mat	mat3;
	t_mat	mat4;

	mat1 = (t_mat){ (t_fvec3){ 0.3, 0.6, 0.1 } }; //orange
	mat2 = (t_mat){ (t_fvec3){ 0.4, 0.4, 0.3 } }; //yellow
	mat3 = (t_mat){ (t_fvec3){ 0.2, 0.2, 0.9 } }; //green
	mat4 = (t_mat){ (t_fvec3){ 0.1, 0.4, 0.7 } }; //red
	sphere_arr = (t_sphr *)malloc(sizeof(t_sphr) * SPHERE_CNT);
//	sphere_arr[0] = (t_sphr){ (t_fvec3){ 5, 5, -16 }, 1 };
//	sphere_arr[1] = (t_sphr){ (t_fvec3){ 10, 10, -16 }, 1 };
//	sphere_arr[2] = (t_sphr){ (t_fvec3){ 20, 20, -16 }, 1 };
//	sphere_arr[3] = (t_sphr){ (t_fvec3){ 30, 30, -16 }, 1 };
//	sphere_arr[4] = (t_sphr){ (t_fvec3){ 50, 50, -16 }, 1 };



	sphere_arr[0] = (t_sphr){ (t_fvec3){ 0, 0, 16 }, 1, mat1 };
	sphere_arr[1] = (t_sphr){ (t_fvec3){ 5, 5, 16 }, 1, mat2 };
	sphere_arr[2] = (t_sphr){ (t_fvec3){ -10, 10, 16 }, 1, mat3 };
	sphere_arr[3] = (t_sphr){ (t_fvec3){ 10, -10, 19 }, 1, mat4 };
//	sphere_arr[4] = (t_sphr){ (t_fvec3){ }, 1 };



	while (!env->flags.is_game_over && prepare_render(env, &env->fps))
	{
		//clear_img_buff(env);
		float x;
		float y;
		for (size_t j = 0; j < WIN_HEIGHT; j++)
		{
			for (size_t i = 0; i < WIN_WIDTH; i++)
			{
				x = (2 * (i + 0.5) / (float)WIN_WIDTH - 1) * tan(FOV / 2.)
									* WIN_WIDTH / (float)WIN_HEIGHT;
				y = -(2 * (j + 0.5) / (float)WIN_HEIGHT - 1) * tan(FOV / 2.);
				t_fvec3 dir;
				dir = (t_fvec3){ x, y, -1 };
				vec3_normalize(&dir, &dir);
				t_fvec3 tmp = (t_fvec3){ 0, 0, 0 };
				framebuffer[i + j * WIN_WIDTH] =
					cast_ray(&tmp, &dir, sphere_arr);
			}
		}
		for (size_t i = 0; i < WIN_HEIGHT * WIN_WIDTH; ++i)
		{
			((uint32_t *)env->buff)[i] =
					double2byte_clamp(framebuffer[i].x) << (1u * 8u)
				| double2byte_clamp(framebuffer[i].y) << (2u * 8u)
				| double2byte_clamp(framebuffer[i].z) << (3u * 8u);
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
