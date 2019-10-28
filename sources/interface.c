/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 14:11:50 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/28 20:47:18 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char			*draw_text(t_env *env, t_ivec pos, char *text)
{
	SDL_Color			color;
	SDL_Surface*		msg;
	SDL_Texture*		message;
	SDL_Rect			msgRect;

	color = (SDL_Color){255, 255, 255, 0};
	msg = TTF_RenderText_Solid(env->font, text, color);
	message = SDL_CreateTextureFromSurface(env->renderer, msg);
	msgRect = (SDL_Rect){pos.x, pos.y, msg->w, msg->h};
	SDL_RenderCopy(env->renderer, message, NULL, &msgRect);
	SDL_FreeSurface(msg);
	SDL_DestroyTexture(message);
	return (text);
}

static void		show_menu(t_env *env, t_ivec *pos, t_ivec offset)
{
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y},
		"select_mod: caps_lock");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "sepia: 1");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "grayscale: 2");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "is_camera_mod: 3");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "screenshot: 4");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "reset: R");
	draw_text(env, (t_ivec){pos->x, pos->y += offset.y}, "hide menu: M");
}

void			show_inteface(t_env *env)
{
	t_ivec	leg;
	t_ivec	offset;

	if (!env)
		return ;
	leg = (t_ivec){20, 20};
	offset = (t_ivec){80, 30};
	if (env->flags.is_info)
		show_info(env, &leg, offset);
	else
		draw_text(env, (t_ivec) {leg.x, leg.y}, "show object info: I");
	if (env->flags.is_menu)
		show_menu(env, &leg, offset);
	else
		draw_text(env, (t_ivec){leg.x, leg.y += offset.y}, "show menu: M");
}
