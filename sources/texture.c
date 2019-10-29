/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 10:04:51 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 14:12:19 by dromanic         ###   ########.fr       */
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

void		init_img_tex(t_env *env, SDL_Surface **img_tex)
{
	img_tex[0] = load_surface(env, "resources/textures/1.jpg");
	img_tex[1] = load_surface(env, "resources/textures/2.jpg");
	img_tex[2] = load_surface(env, "resources/textures/3.png");
}

Uint32		set_color_img(SDL_Surface *img, int x, int y)
{
	int		byt;
	uint8_t	*pix;

	(x < 0) ? x = 0 : 0;
	if (!img)
		return (0);
	byt = img->format->BytesPerPixel;
	pix = (uint8_t *)img->pixels + y * img->pitch + x * byt;
	if (byt == 1)
		return (*pix);
	if (byt == 2)
		return (*(uint16_t *)pix);
	if (byt == 3)
	{
		if (SDL_BYTEORDER != SDL_BIG_ENDIAN)
			return (pix[0] << 16 | pix[1] << 8 | pix[2]);
		else
			return (pix[0] | pix[1] << 8 | pix[2] << 16);
	}
	if (byt == 4)
		return (*(uint32_t *)pix);
	return (0);
}

//p = sphere touch_point,  (t_sphere *sp obj_color, tex_arr[tex_id])
//int			set_img_cord_to_sphere(t_v3d p, t_sphere *sp)
//{
//	Uint32	*pix;
//	double	u;
//	double	v;
//
//	pix = NULL;
//	if (sp->img == NULL)
//		return (sp->color.color);
//	vec_3normalize(&p);
//	u = 0.5 + atan2(p.z, p.x) / (2 * PI);
//	v = 0.5 - asin(p.y) / PI;
//	pix = (Uint32*)sp->img->pixels;
//	return (set_color_img(sp->img,
//		(int)(u * sp->img->w),
//		(int)(v * sp->img->h)));
//}

void			texturing_or_color(t_lght_comp *l, const t_env *env,
	const t_ray *ray, t_uni *obj)
{
	t_dvec3		sphere_dir;
	double		u;
	double		v;
	Uint32		pixel;

	if (obj->get_intersect == get_intersect_sphere)
	{
		sphere_dir = ray->normal;
		u = 0.5 + atan2(sphere_dir.z, sphere_dir.x) / (2 * M_PI);
		v = 0.5 - asin(sphere_dir.y) / M_PI;
		pixel = (set_color_img(env->tex_arr[obj->texture_id],
			(int)(u * env->tex_arr[obj->texture_id]->w),
			(int)(v * env->tex_arr[obj->texture_id]->h)));
		l->obj_color = (t_dvec3){
			.x = (pixel & 0xff0000u) >> 16u,
			.y = (pixel & 0x00ff00u) >> 8u,
			.z = (pixel & 0x0000ffu)};
	}
	else
		l->obj_color = obj->color;
}