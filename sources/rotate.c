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

void	rotate_x(t_dvec3 *destination,
					const t_dvec3 *restrict pt,
					double angle)
{
	*destination = (t_dvec3){.x = pt->x * cos(angle) + pt->z * sin(angle),
							.y = pt->y,
							.z = -pt->x * sin(angle) + pt->z * cos(angle)};
}

void	rotate_y(t_dvec3 *destination,
					const t_dvec3 *restrict pt,
					double angle)
{
	*destination = (t_dvec3){.x = pt->x,
							.y = pt->y * cos(angle) + pt->z * sin(angle),
							.z = -pt->y * sin(angle) + pt->z * cos(angle)};
}

void	rotate_z(t_dvec3 *destination,
					const t_dvec3 *restrict pt,
					double angle)
{
	*destination = (t_dvec3){.x = pt->x * cos(angle) - pt->y * sin(angle),
							.y = pt->x * sin(angle) + pt->y * cos(angle),
							.z = pt->z};
}

void	rotate_vec(t_dvec3 *dir, t_dvec3 *rotate_angle)
{
	if (rotate_angle->x != 0.0)
		rotate_x(dir, dir, rotate_angle->x * M_PI / 180);
	if (rotate_angle->y != 0.0)
		rotate_y(dir, dir, rotate_angle->y * M_PI / 180);
	if (rotate_angle->z != 0.0)
		rotate_z(dir, dir, rotate_angle->z * M_PI / 180);
}
