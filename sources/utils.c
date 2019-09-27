/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:43:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/27 18:16:54 by dromanic         ###   ########.fr       */
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



void print_error(int err_nb, int row)
{
	system("leaks -q RTv1");
	printf("\n");

	if(row >= 0)
	{
		ft_putstr("Line ");
		ft_putnbr(row + 1);
		ft_putstr(": ");
	}
	if (err_nb == ERR_MALLOC)
		ft_putendl("Malloc failed");
	else if (err_nb == ERR_SDL)
	{
		ft_putendl("SDL failed:");
		ft_putendl(SDL_GetError());
	}
	else if (err_nb == ERR_USAGE)
		ft_putendl("Usage : ./RTv1 <the scenne>");
	else if (err_nb == ERR_WRONG_SYMBOL)
		ft_putendl("Detected invalid symbol");
	else if (err_nb == ERR_LINE)
		ft_putendl("Missed some parameters");
	else if (err_nb == ERR_ARGV)
		ft_putendl("Invalid argument or empty scenne");
	else if (err_nb == ERR_PIPE)
		ft_putendl("Too many '|' in line");
	else if (err_nb == ERR_DOT)
		ft_putendl("No number around '.' or too many '.' in the number");
	else if (err_nb == ERR_NBR)
		ft_putendl("Wrong number of numbers");
	else if (err_nb == ERR_MINUS)
		ft_putendl("Wrong  symbol's '-' position");
	else if (err_nb == ERR_LEN)
		ft_putendl("String contains more than 200 characters");
	else if (err_nb == ERR_ROWS)
		ft_putendl("Too many rows in the file");
	exit(err_nb);
}
