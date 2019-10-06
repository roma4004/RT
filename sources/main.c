/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/05 13:34:11 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

uint8_t			double_clamp(double x)
{
	if (x < 0.0)
		return (0);
	else if (x > 255.0)
		return (255);
	else
		return ((uint8_t)(x));
}

static void		show_errors(t_env *env)
{
	if (env->err_id == 404)
		ft_putstr_fd("SCENE_ERR", 2);
	if (env->err_id == 405)
		ft_putstr_fd("READ_ERR", 2);
	if (env->err_id == 406)
		ft_putstr_fd("SCENE_SIZE_ERR", 2);
	if (env->err_id && errno)
		ft_putstr_fd(" - ", 2);
	if (errno)
		ft_putstr_fd(strerror(errno), 2);
	if (env->err_id || errno)
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
	if (env->surface)
		SDL_FreeSurface(env->surface);
	env->surface = NULL;
	show_errors(env);
	SDL_Quit();
}

int				main(int argc, char **argv)
{
	t_env	*env;

	if (argc == 2)
	{
		if ((env = init_env()) && parse_scene(env, argv[1]))
		{
			draw_scene(env, env->threads);
			while (!(env->is_rtv1_over))
				if (event_handler(env, &env->cam, &env->flags))
					draw_scene(env, env->threads);
		}
		else
			quit_program(env);
	}
	else
		ft_putendl("Usage : ./RTv1 scenne_file");
	return (0);
}
