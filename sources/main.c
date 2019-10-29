/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/18 17:13:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 21:12:48 by dromanic         ###   ########.fr       */
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
		ft_putstr_fd("ERR_SCENE", 2);
	if (env->flags.err_id == 405)
		ft_putstr_fd("ERR_READ", 2);
	if (env->flags.err_id == 406)
		ft_putstr_fd("ERR_SIZE", 2);
	if (env->flags.err_id == 407)
		ft_putstr_fd("INVALID_RESOURCE", 2);
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
	if (env->font)
		TTF_CloseFont(env->font);
	env->window = NULL;
	if (env->buff)
		free(env->buff);
	if (env->tex_arr)
		free(env->tex_arr);
	if (env->uni_arr)
		free(env->uni_arr);
	if (env->neg_arr)
		free(env->neg_arr);
	show_errors(env);
	SDL_Quit();
}

void			screen_update(t_env *env)
{
	if (!env)
		return ;
	SDL_UpdateTexture(env->screen, NULL, env->buff, env->buff_width << 2u);
	SDL_RenderCopy(env->renderer, env->screen, NULL, NULL);
	if (!env->flags.is_camera_mod)
		show_inteface(env);
	SDL_RenderPresent(env->renderer);
}

int				main(int argc, char **argv)
{
	t_env	*env;

	if (argc == 2)
	{
		if ((env = init_env())
		&& parse_switch(env, argv[1])
		&& init_sdl2(env))
		{
			draw_scene(env, env->threads);
			while (!(env->flags.is_rtv1_over))
				if (event_handler(env, &env->cam, &env->flags))
					draw_scene(env, env->threads);
		}
		else
			quit_program(env);
	}
	else
		ft_putendl("Usage : ./RT scene_file");
	system("leaks -q RT");
	return (0);
}
