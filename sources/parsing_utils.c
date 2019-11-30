/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 14:49:14 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 20:59:30 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		cnt_neg_obj(t_env *env, uint64_t type)
{
	env->neg_arr_len++;
	if (type == CYLINDERNEG)
		env->neg_arr_len += 2u;
	if (type == CONENEG)
		env->neg_arr_len++;
}

static void		cnt_uni_obj(t_env *env, uint64_t type)
{
	env->uni_arr_len++;
	if (type == CYLINDER)
		env->uni_arr_len += 2u;
	if (type == CONE)
		env->uni_arr_len++;
}

static void		cnt_obj_type(t_env *env, t_list *lst)
{
	t_list		*cur;
	uint64_t	type;

	cur = lst;
	while (cur)
	{
		if ((type = get_type(cur->content)) != UINT64_MAX)
		{
			if (type < 3u)
				env->light_arr_len++;
			else if (type >= OBJ_TYPE_MAX && type < SCRN)
				cnt_neg_obj(env, type);
			else if (type < OBJ_TYPE_MAX)
				cnt_uni_obj(env, type);
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

_Bool			parse_switch(t_env *env, char *file_name)
{
	if (ft_strnstr(file_name, ".json", ft_strlen(file_name)))
		return (json_parson(env, file_name, &env->flags.err_id));
	return (parse_scene(env, file_name));
}
