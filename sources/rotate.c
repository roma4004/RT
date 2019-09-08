/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 14:59:52 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/08 16:39:50 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "math.h"

t_fvec3		rotate_y(t_fvec3 pt, double angle)
{
	return ((t_fvec3){ pt.x,
						pt.y * cos(angle) + pt.z * sin(angle),
						-pt.y * sin(angle) + pt.z * cos(angle) });
}

t_fvec3		rotate_x(t_fvec3 pt, double angle)
{
	return ((t_fvec3){ pt.x * cos(angle) + pt.z * sin(angle),
						pt.y,
						-pt.x * sin(angle) + pt.z * cos(angle) });
}

t_fvec3		rotate_z(t_fvec3 pt, double angle)
{
	return ((t_fvec3){ pt.x * cos(angle) - pt.y * sin(angle),
						pt.x * sin(angle) + pt.y * cos(angle),
						pt.z });
}


void			rotate_cam(t_env *env)
{
	if (env->cam.rotate_angle.x != 0)
		env->cam.dir = rotate_x(env->cam.dir, env->cam.rotate_angle.x * M_PI
		/ 180);
	if (env->cam.rotate_angle.y != 0)
		env->cam.dir = rotate_y(env->cam.dir, env->cam.rotate_angle.y * M_PI
		/ 180);
	if (env->cam.rotate_angle.z != 0)
		env->cam.dir = rotate_z(env->cam.dir, env->cam.rotate_angle.z * M_PI
		/ 180);
}

