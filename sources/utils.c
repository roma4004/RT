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

t_fvec3		convert_to_viewport(float x, float y)
{
	return ((t_fvec3){ x * VIEWPORT_SIZE / WIN_WIDTH,
					y * VIEWPORT_SIZE / WIN_HEIGHT,
					PROJECTION_PLANE_Z });
}
