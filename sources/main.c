/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 21:27:29 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double			double_clamp(double x)
{
	if (x < 0.0)
		return (0);
	else if (x > 255.0)
		return (255.0);
	else
		return (x);
}

static void		show_errors(t_env *env)
{
	if (env->flags.err_id == 404)
		ft_putstr_fd("SCENE_ERR", 2);
	if (env->flags.err_id == 405)
		ft_putstr_fd("READ_ERR", 2);
	if (env->flags.err_id == 406)
		ft_putstr_fd("SCENE_SIZE_ERR", 2);
	if (env->flags.err_id && errno)
		ft_putstr_fd(" - ", 2);
	if (errno)
		ft_putstr_fd(strerror(errno), 2);
	if (env->flags.err_id || errno)
		ft_putstr_fd("\n", 2);
	if (DEBUG)
	{
		if (ft_strcmp(SDL_GetError(), ""))
		{
			ft_putstr("SDL_LAST_ERROR - ");
			ft_putstr(SDL_GetError());
		}
		ft_putchar('\n');
	}
}

void			quit_program(t_env *env)
{
	if (!env)
		return ;
	if (env->screen)
		SDL_DestroyTexture(env->screen);
	env->screen = NULL;
	if (env->renderer)
		SDL_DestroyRenderer(env->renderer);
	env->renderer = NULL;
	if (env->window)
		SDL_DestroyWindow(env->window);
	env->window = NULL;
	show_errors(env);
	SDL_Quit();
}

///checklist:
//+ no recalculation without need
//+ 4 basic object (sphere, plane, cylinder, cone)
//+ camera movement and rotating and placing on the scene on start from scene_file
//+ Multi thread computing
//+ Interacting with objects
//+ selecting object by mouse
//+ sepia filter
//+ It's possible inside the RT to save, screenshot the rendered image. ()

//- colored light
//- JSON
//+- limited obj (by vtlostiu) almost done
//- sliced obj
//- interface
//- composed elements (grouped obj)
//- parse screen param from file (screen obj)
//- texture (checkmate_board)
//- more figures

//optional:
//- blinded by light spot facing us.
//- negative objects
//- add figure paraboloid et hyperboloid.
//- torus

int				main(int argc, char **argv)
{
	t_env	*env;

	if (argc == 2)
	{
		if ((env = init_env())
		&& parse_scene(env, argv[1])
		&& init_sdl2(env))
		{
			//todo: simplify main thread below to 3 func
			///events()
			///updade()
			///render()
			draw_scene(env, env->threads);
			while (!(env->flags.is_rtv1_over))
				if (event_handler(env, &env->cam, &env->flags))
					draw_scene(env, env->threads);
			system("leaks RT");
		}
		else
			quit_program(env);
	}
	else
		ft_putendl("Usage : ./RTv1 scene_file");
	return (0);
}
