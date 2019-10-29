/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 10:04:51 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 21:00:20 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
		env->flags.err_id = INVALID_RESOURCE;
		return (NULL);
	}
	SDL_FreeSurface(new_srf);
	return (convert_srf);
}

void					init_img_tex(t_env *env, SDL_Surface **img_tex)
{
	img_tex[0] = load_surface(env, "resources/textures/1.jpg");
	img_tex[1] = load_surface(env, "resources/textures/2.jpg");
	img_tex[2] = load_surface(env, "resources/textures/3.png");
	img_tex[3] = load_surface(env, "resources/textures/4.jpg");
}

void					texturing_or_color(t_lght_comp *l, const t_env *env,
							const t_ray *ray, t_uni *obj)
{
	SDL_Surface		*tex;
	uint8_t			*pix;
	t_dvec			uv;
	t_ivec			coord;
	Uint32			pixel;

	if (obj->get_intersect == get_intersect_sphere
	&& obj->texture_id < env->tex_arr_len && obj->texture_id > -1)
	{
		uv = (t_dvec){
			.x = 0.5 + atan2(ray->normal.z, ray->normal.x) / (2 * M_PI),
			.y = 0.5 - asin(ray->normal.y) / M_PI};
		tex = env->tex_arr[(size_t)obj->texture_id];
		coord = (t_ivec){.x = (int)(uv.x * tex->w), .y = (int)(uv.y * tex->h)};
		pix = (uint8_t *)tex->pixels
			+ coord.y * tex->pitch
			+ coord.x * tex->format->BytesPerPixel;
		pixel = *(uint32_t *)pix;
		l->obj_color = (t_dvec3){
			.x = (pixel & 0xff0000u) >> 16u,
			.y = (pixel & 0x00ff00u) >> 8u,
			.z = (pixel & 0x0000ffu)};
	}
	else
		l->obj_color = obj->color;
}
