/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/26 14:37:51 by dromanic         ###   ########.fr       */
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


//cur->pos
//cur->rotate_state{x,y,z}
//cur->color
//cur->is_reflected
//cur->is_refracted
//cur->is_light_source
//cur->name;
//cur->type;
//cur->opacity;


//int				main(int argc, char **argv)
int				main(void)
{
	t_env *env;
	env = init_env();
	rerender_scene(env);
	return (0);
}
