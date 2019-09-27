/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/27 18:16:11 by dromanic         ###   ########.fr       */
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

int				main(int argc, char **argv)
{
	t_env *env;

	env = init_env();


//	int fd = open(argv[1], O_RDONLY);
//	validation_file(env, fd);
//	close(fd);
//	read_data(env);


	rerender_scene(env);
	while (!env->flags.is_rtv1_over)
		if (event_handler(&env->cam, &env->flags))
			rerender_scene(env);
	return (0);
}
