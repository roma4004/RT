/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 14:59:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 13:48:11 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static t_dvec3		rotate_y(t_dvec3 *pt, double angle)
{
	return ((t_dvec3){pt->x,
						pt->y * cos(angle) + pt->z * sin(angle),
						-pt->y * sin(angle) + pt->z * cos(angle), 0});
}

static t_dvec3		rotate_x(t_dvec3 *pt, double angle)
{
	return ((t_dvec3){ pt->x * cos(angle) + pt->z * sin(angle),
						pt->y,
						-pt->x * sin(angle) + pt->z * cos(angle), 0});
}

static t_dvec3		rotate_z(t_dvec3 *pt, double angle)
{
	return ((t_dvec3){pt->x * cos(angle) - pt->y * sin(angle),
						pt->x * sin(angle) + pt->y * cos(angle),
						pt->z, 0});
}

void				rotate_cam(t_dvec3 *dir, t_dvec3 *rotate_angle)
{
	if (rotate_angle->x != 0.0)
		*dir = rotate_x(dir, rotate_angle->x * M_PI / 180);
	if (rotate_angle->y != 0.0)
		*dir = rotate_y(dir, rotate_angle->y * M_PI / 180);
	if (rotate_angle->z != 0.0)
		*dir = rotate_z(dir, rotate_angle->z * M_PI / 180);
}
