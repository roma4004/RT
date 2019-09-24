/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:43:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/08 14:43:09 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

t_dvec3		convert_to_viewport(double x, double y)
{
	return ((t_dvec3){ x * VIEWPORT_SIZE / WIN_WIDTH,
					y * VIEWPORT_SIZE / WIN_HEIGHT,
					DISTANCE_TO_PLANE });
}
