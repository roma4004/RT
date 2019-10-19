/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 20:50:03 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 14:46:33 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		apply_sepia_filter(t_dvec3 *color)
{
	*color = (t_dvec3){
		.x = (color->x * 0.393) + (color->y * 0.769) + (color->z * 0.189),
		.y = (color->x * 0.349) + (color->y * 0.686) + (color->z * 0.168),
		.z = (color->x * 0.272) + (color->y * 0.534) + (color->z * 0.131)};
	ft_clamp_in_range_vec(color, 0, 255);
}

static void		apply_grayscale_filter(t_dvec3 *color)
{
	*color = (t_dvec3){
		.x = (color->x + color->y + color->z) / 3,
		.y = (color->x + color->y + color->z) / 3,
		.z = (color->x + color->y + color->z) / 3};
}

void			apply_effects(t_flags *flags, t_dvec3 *color)
{
	if (flags->is_sepia)
		apply_sepia_filter(color);
	if (flags->is_grayscale)
		apply_grayscale_filter(color);
}

static void		replace_space_newline(char *str)
{
	size_t		i;

	i = UINT64_MAX;
	while (str[++i] && (str[i] = ft_tolower(str[i])))
		if (str[i] == ' ' || str[i] == '\n')
			str[i] = '_';
}

void			save_screenshot(t_env *env)
{
	SDL_Surface		*screenshot;
	time_t			rawtime;
	struct tm		*timeinfo;
	char			*str;
	char			*tmp;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	tmp = ft_strjoin("screenshot_RT_", asctime(timeinfo));
	str = ft_strjoin(tmp, ".bmp");
	ft_strdel(&tmp);
	replace_space_newline(str);
	screenshot = SDL_CreateRGBSurface(0, WIN_WIDTH, WIN_HEIGHT, 32, 0, 0, 0, 0);
	SDL_LockSurface(screenshot);
	ft_memcpy(screenshot->pixels, env->buff, screenshot->h * screenshot->pitch);
	SDL_UnlockSurface(screenshot);
	SDL_SaveBMP(screenshot, str);
//	IMG_SavePNG(screenshot, str);
	ft_strdel(&str);
	SDL_FreeSurface(screenshot);
}
