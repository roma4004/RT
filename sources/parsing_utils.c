/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 14:49:14 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/24 16:23:19 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		cnt_obj_type(t_env *env, t_list *lst)
{
	t_list		*cur;
	size_t		type;

	cur = lst;
	while (cur)
	{
		if ((type = get_type(cur->content)) != UINT64_MAX)
		{
			if (type == SPHERENEG)
				env->neg_arr_len++;
			else if (type < 3u)
				env->light_arr_len++;
			else if (type < OBJ_TYPE_MAX)
			{
				env->uni_arr_len++;
				if (type == CYLINDER)
					env->uni_arr_len += 2u;
				if (type == CONE)
					env->uni_arr_len++;
			}
		}
		cur = cur->next;
	}
}

_Bool			init_obj_arr(t_env *env, t_list *lst)
{
	cnt_obj_type(env, lst);
	if ((env->uni_arr = (t_uni *)malloc(sizeof(t_uni) * env->uni_arr_len))
	&& (env->light_arr = (t_lght *)malloc(sizeof(t_lght) * env->light_arr_len))
	&& (env->neg_arr = (t_uni *)malloc(sizeof(t_uni) * env->neg_arr_len)))
		return (true);
	else
	{
		if (env->uni_arr)
			ft_memdel((void **)&env->uni_arr);
		if (env->light_arr)
			ft_memdel((void **)&env->light_arr);
		if (env->neg_arr)
			ft_memdel((void **)&env->neg_arr);
	}
	return (false);
}
