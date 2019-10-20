/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 14:49:14 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/20 15:14:56 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		cnt_obj_type(t_env *env, t_list *lst)
{
	size_t		*lights;
	size_t		*objs;
	t_list		*cur;
	size_t		type;

	lights = &env->light_arr_len;
	objs = &env->uni_arr_len;
	cur = lst;
	while (cur)
	{
		if ((type = get_type(cur->content)) != UINT64_MAX)
		{
			if (type < 3u)
				(*lights)++;
			else if (type < OBJ_TYPE_MAX)
			{
				(*objs)++;
				if (type == CYLINDER)
					(*objs) += 2;
				if (type == CONE)
					(*objs)++;
			}
		}
		cur = cur->next;
	}
}

_Bool			init_obj_arr(t_env *env, t_list *lst)
{
	cnt_obj_type(env, lst);
	if ((env->uni_arr = (t_uni *)malloc(sizeof(t_uni) * env->uni_arr_len))
	&& (env->light_arr = (t_lght *)malloc(sizeof(t_lght) * env->light_arr_len)))
		return (true);
	else
	{
		if (env->uni_arr)
			ft_memdel((void **)&env->uni_arr);
		if (env->light_arr)
			ft_memdel((void **)&env->light_arr);
	}
	return (false);
}
