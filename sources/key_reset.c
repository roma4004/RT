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
	t_uni		*obj;
	size_t		i;

	if (obj_cnt)
	{
		i = UINT64_MAX;
		while (++i < env->uni_arr_len && (obj = &env->uni_arr[i]))
		{
			if (obj->is_selected)
			{
				obj->pos = obj->pos_backup;
				obj->dir = obj->dir_backup;
			}
		}
	}
	else
	{
		cam->pos = cam->pos_backup;
		cam->rotate_angle = cam->rotate_angle_backup;
	}
}
