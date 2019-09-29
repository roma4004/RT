/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 15:29:51 by dromanic         ###   ########.fr       */
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

int				main(int argc, char **argv)
{
	t_env *env;
	//todo: err hangling
	env = init_env();
	if (argc != 1 && parse_scene(env, argv[1]))
	{
		rerender_scene(env);
		while (!env->flags.is_rtv1_over)
			if (event_handler(&env->cam, &env->flags))
				rerender_scene(env);
	}
	else
		ft_putendl("Usage : ./RTv1 scenne_file");
	return (0);
}
