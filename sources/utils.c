/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:43:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/28 12:02:26 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static uint8_t  double2byte_clamp(const double x)
{
	if (x < 0.0)
		return (0);
	else if (x > 1.0)
		return (255);
	else
		return ((uint8_t)(255.0 * x));
}

uint32_t		vec3_to_color(const t_dvec3 *restrict first)
{
	return (double2byte_clamp(first->x) << (1u * 8u)
		| double2byte_clamp(first->y) << (2u * 8u)
		| double2byte_clamp(first->z) << (3u * 8u));
}

t_dvec3			discriminant_comput(t_dvec3 *tmp)
{
	const double	discriminant = tmp->y * tmp->y - 4 * tmp->x * tmp->z;

	if (discriminant < 0.0)
		return ((t_dvec3){MAXFLOAT, MAXFLOAT, 0.0});
	return ((t_dvec3){(-tmp->y + sqrtf(discriminant)) / (2.0 * tmp->x),
					  (-tmp->y - sqrtf(discriminant)) / (2.0 * tmp->x), 0.0});
}



void print_error(int err_nb, unsigned row)
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
