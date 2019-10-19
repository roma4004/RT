/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 19:00:15 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 19:05:59 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		swith_handle(t_env *env, t_flags *flags, size_t obj_cnt)
{
	if (flags->is_reset)
		reset(env, &env->cam, obj_cnt);
	if (flags->is_screenshot)
		save_screenshot(env);
}
void		count_selected_obj(size_t *dest, t_uni *uni_arr, size_t uni_arr_len)
{
	size_t		obj_cnt;
	size_t		i;

	i = UINT64_MAX;
	obj_cnt = 0;
	while (++i < uni_arr_len)
		if (uni_arr[i].is_selected)
			obj_cnt++;
	*dest = obj_cnt;
}