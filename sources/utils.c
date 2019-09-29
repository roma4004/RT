/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:43:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 11:55:29 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void			discriminant_comput(t_dvec3 *tmp, t_dvec3 *plane_toch)
{
	const double	discriminant = tmp->y * tmp->y - 4 * tmp->x * tmp->z;

	if (discriminant < 0.0)
		*plane_toch = (t_dvec3){MAXFLOAT, MAXFLOAT, 0.0};
	*plane_toch = (t_dvec3){(-tmp->y + sqrtf(discriminant)) / (2.0 * tmp->x),
						(-tmp->y - sqrtf(discriminant)) / (2.0 * tmp->x), 0.0};
}

void			print_error(int err_nb, unsigned row)
{
	system("leaks -q RTv1");
	printf("\n");
	if(row)
	{
		ft_putstr("Error in line ");
		ft_putnbr(row + 1);
	}
//	if (err_nb == ERR_SDL)
//	{
//		ft_putendl("SDL failed:");
//		ft_putendl(SDL_GetError());
//	}
	exit(err_nb);
}

void			set_value(t_env *env, const double *v, int type)
{
	static size_t	id_uni = 0;
	static size_t	id_lgh = 0;

	if (type == UINT64_MAX)
		return ;
	if (type == 7)
	{
		env->cam.pos = (t_dvec3){v[0], v[1], v[2]};
		env->cam.rotate_angle = (t_dvec3){v[4], v[5], v[6]};
	}
	else if (type < 3)
	{
		env->light_arr[id_lgh++] =
			(t_lght){type, (t_dvec3){v[0], v[1], v[2]}, v[3]};
	}
	else
	{
		env->uni_arr[id_uni] =
			(t_uni){type, (t_dvec3){v[0], v[1], v[2]}, v[3],
					(t_dvec3){v[4], v[5], v[6]}, (t_dvec3){v[7], v[8], v[9]}, v[10]
			};
		env->uni_arr[id_uni].dir = vec3_normalize(env->uni_arr[id_uni].dir);
		id_uni++;
	}
}