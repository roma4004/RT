/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/07 20:19:18 by dromanic         ###   ########.fr       */
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
//	show_errors(env);
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
	if ((fps->frame_limit_second) > fps->cur_tick - fps->pre_tick)
		SDL_Delay(fps->frame_limit_second - (fps->cur_tick - fps->pre_tick));
	event_handler(env, &env->cam, &env->flags);
	clear_img_buff(env);
	return (1);
}

//void			game_loop(t_env *env, int threads)
//{
//	int			id;
//	t_pth_dt	data[threads];
//	pthread_t	threads_arr[threads];
//
//	if (!env)
//		return ;
//	while (!env->flags.is_game_over && prepare_render(env, &env->fps))
//	{
//		id = -1;
//		while (++id < threads)
//		{
//			data[id].env = env;
//			data[id].offset = id;
//			pthread_create(&threads_arr[id], NULL, multi_raycasting, &data[id]);
//		}
//		id = -1;
//		while (++id < threads)
//			pthread_join(threads_arr[id], NULL);
//		SDL_UpdateTexture(env->screen, NULL, env->buff, WIN_WIDTH << 2);
//		SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);
//		if (SHOW_FPS)
//			render_interface(env, &env->fps, &env->txt);
//		SDL_RenderPresent(env->renderer);
//	}
//}

//normal = a * b
//cosA = ( |a| * |b| ) / ( a * b )








//sounds https://www.deezer.com/track/441682922?utm_source=deezer&utm_content=track-441682922&utm_term=2309518464_1565535009&utm_medium=web


//recurcive or iterated spawn of reflected and refracted rays
//
// list->list of rays which saves spot, when ray spawn new two rays reflect
// and refract

//ray_max_bounced

//list-

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



//float	fclamp(float arg, float minimum, float maximum)
//{
//	return (fminf(fmaxf(arg, minimum), maximum));
//}
//static uint8_t  double2byte_clamp(const double x)
//{
//	if (x < 0.0)
//		return (0);
//	else if (x > 1.0)
//		return (255);
//	else
//		return ((uint8_t)(255.0 * x));
//}

////Sphere(const Vec3f &c, const float &r) : center(c), radius(r) {}
//bool ray_intersect(const t_sphr *sphere, const t_fvec3 *orig,
//					const t_fvec3 *dir, float *t0)
//{
//	float		thc;
//	float		tca;
//	t_fvec3		L;
//	float		d2;
//	t_fvec3		tmp;
//
//	vec3_sub_vec3(&L, &sphere->center, orig);
//	vec3_mul_vec3(&tmp, &L, dir);
//	tca = vec3_to_float(&tmp);
//	vec3_mul_vec3(&tmp, &L, &L);
//	vec3_sub_float(&tmp, &tmp, powf(tca, 2));
//	d2 = vec3_to_float(&tmp);
//	if (d2 > powf(sphere->radius, 2))
//		return false;
//	thc = sqrtf(powf(sphere->radius, 2) - d2);
//	*t0 = tca - thc;
//	float t1 = tca + thc;
//	if (*t0 < 0)
//		*t0 = t1;
//	if (*t0 < 0)
//		return false;
//	return true;
//}

//bool scene_intersect(const t_fvec3 *orig, const t_fvec3 *dir,
//		const t_sphr *spheres, t_fvec3 *hit, t_fvec3 *N, t_mat *material)
//{
//	float		spheres_dist;
//	float		dist_i;
//	t_fvec3		tmp;
//
//	spheres_dist = MAXFLOAT;//std::numeric_limits<float>::max();
//	for (size_t i = 0; i < SPHERE_CNT; i++)
//	{
//		if (ray_intersect(&spheres[i], orig, dir, &dist_i)
//		&& dist_i < spheres_dist)
//		{
//			spheres_dist = dist_i;
//			vec3_mul_float(&tmp, dir, dist_i);
//			vec3_add_vec3(hit, orig, &tmp);
//			vec3_sub_vec3(&tmp, hit, &spheres[i].center);
//			vec3_normalize(N, &tmp);
//			*material = spheres[i].material;
//		}
//	}
//	return (spheres_dist < 1000);
//}

static float			ft_max(float first, float second)
{
	return (first > second ? first : second);
}

//t_fvec3			cast_ray(const t_fvec3 *orig, const t_fvec3 *dir,
//							const t_sphr *sphere_arr,
//							const t_lght *lights_arr)
//{
//	t_fvec3 point;
//	t_fvec3 N;
//	t_mat   material;
//
//	if (!scene_intersect(orig, dir, sphere_arr, &point, &N, &material))
//	{		//			B		G		R
//		return (t_fvec3){ 0.2, 0.7, 0.8 }; // background color
//	}
//	float diffuse_light_intensity = 0;
//	for (size_t i = 0; i < LIGHTS_CNT; i++)
//	{
//		t_fvec3 tmp;
//		vec3_sub_vec3(&tmp, &lights_arr[i].position, &point);
//		t_fvec3 light_dir = (t_fvec3){ 0, 0, 0 };
//		vec3_normalize(&light_dir, &tmp);
//		vec3_mul_vec3(&tmp, &light_dir, &N);
//		diffuse_light_intensity +=
//			lights_arr[i].intensity * ft_max(0.f, vec3_to_float(&tmp));
//	}
//	t_fvec3 tmp;
//	vec3_mul_float(&tmp, &material.diffuse_color, diffuse_light_intensity);
//	return tmp;
////	return material.diffuse_color;
//}
//
//uint32_t		vec3_to_color(const t_fvec3 *restrict first)
//{
//	return (  double2byte_clamp(first->x) << (1u * 8u)
//			| double2byte_clamp(first->y) << (2u * 8u)
//			| double2byte_clamp(first->z) << (3u * 8u));
//}

static  t_fvec3		CanvasToViewport(float x, float y)
{
	return (t_fvec3){ x * VIEWPORT_SIZE / WIN_WIDTH,
					  y * VIEWPORT_SIZE / WIN_HEIGHT,
					  PROJECTION_PLANE_Z };
}

// The PutPixel() function.
static void		PutPixel(t_env *env, float x, float y, t_fvec3 color)
{
	x = WIN_WIDTH / 2.f + x;
	y = WIN_HEIGHT / 2.f - y - 1;
	if (x < 0 || x >= WIN_WIDTH
	|| y < 0 || y >= WIN_HEIGHT)
		return ;
	env->buff[(int)y][(int)x] = ( 255u                 << 24u)
								| (((uint32_t)color.x) << 16u)
								| (((uint32_t)color.y) << 8u)
								| (((uint32_t)color.z));
}

static t_fvec3		where_intersect(t_fvec3 cam, t_fvec3 direction, t_sphr *obj)
{
	const t_fvec3		center = obj->center;
	const float			radius = obj->radius;
	t_fvec3				OC;
	t_fvec3				discr;
	float				discriminant;

	OC = vec3_sub_vec3(cam, center);
	discr.x = vec3_dot_vec3(direction, direction);
	discr.y = vec3_to_float(float_mul_vec3(2, vec3_mul_vec3(OC, direction)));
	discr.z = vec3_dot_vec3(OC, OC) - radius * radius;

	discriminant = discr.y * discr.y - 4 * discr.x * discr.z;
	if (discriminant < 0)
		return (t_fvec3){MAXFLOAT, MAXFLOAT, 0};
	return((t_fvec3){(-discr.y + sqrtf(discriminant)) / (2 * discr.x),
					(-discr.y - sqrtf(discriminant)) / (2 * discr.x), 0});
}

t_sphr				*intersect_obj(t_env *env, float *closest_t)
{
	size_t		i;
	t_sphr		*cur_obj;
	t_fvec3		t;

	cur_obj = NULL;
	i = -1;
	while (++i < SPHERE_CNT)
	{
		t = where_intersect(env->cam.pos, env->cam.dir,
				&env->sphere_arr[i]);
		if (t.x < *closest_t && env->cam.t_min < t.x && t.x < env->cam.t_max
		&& (cur_obj = &env->sphere_arr[i]))
			*closest_t = t.x;
		if (t.y < *closest_t && env->cam.t_min < t.y && t.y < env->cam.t_max
		&& (cur_obj = &env->sphere_arr[i]))
			*closest_t = t.y;
	}
	return (cur_obj);
}

static t_fvec3		send_ray(t_env *env)
{
	float		closest_t;
	t_sphr		*obj;
	t_fvec3		t;
	t_fvec3		normal;

	closest_t = MAXFLOAT;
	if (!(obj = intersect_obj(env, &closest_t)))
		return ((t_fvec3){255, 255, 255});
	t = vec3_add_vec3(env->cam.pos, float_mul_vec3(closest_t, env->cam.dir));
	normal = vec3_sub_vec3(t, obj->center);
	normal = float_mul_vec3(1.0 / Length(normal), normal);
	return (float_mul_vec3(get_light(t, normal, env->light_arr), obj->color));
}

//int				main(int argc, char **argv)
int				main(void)
{
	t_env *env;
	env = init_env();


	//draw
	double y;
	double x;
	while (!env->flags.is_game_over && prepare_render(env, &env->fps))
	{
		x = -env->canvas_half.x - 1;
		while (++x < env->canvas_half.x)
		{
			y = -env->canvas_half.y - 1;
			while (++y < env->canvas_half.y)
			{
				env->cam.dir = CanvasToViewport(x, y);
				t_fvec3 color = send_ray(env);
				PutPixel(env, x, y, color);
			}
		}
		event_handler(env, &env->cam, &env->flags);

		SDL_UpdateTexture(env->screen, NULL, env->buff, WIN_WIDTH << 2u);
		SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);

		SDL_RenderPresent(env->renderer);
	}
	return (0);
}
