/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_reset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 13:31:21 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:33:20 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	reset(t_env *env, t_cam *restrict cam, size_t obj_cnt)
{
	size_t		i;

	if (obj_cnt)
	{
		i = ~0UL;
		while (++i < env->uni_arr_len)
		{
			if (env->uni_arr[i].is_selected)
			{
				env->uni_arr[i].pos = env->uni_arr[i].pos_backup;
				env->uni_arr[i].dir = env->uni_arr[i].dir_backup;
			}
		}
	}
	else
	{
		cam->pos = cam->pos_backup;
		cam->rotate_angle = cam->rotate_angle_backup;
	}
}
