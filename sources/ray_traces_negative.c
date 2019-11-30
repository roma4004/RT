/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_traces_negative.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 21:01:09 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 21:02:36 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static _Bool	is_in_range(double val, double min, double max)
{
	if (val > min && val < max)
		return (true);
	return (false);
}

_Bool			inside_negative_obj(t_list *lst_neg, double val_t)
{
	t_touch		*negative_touch;

	while (lst_neg)
	{
		negative_touch = (t_touch *)lst_neg->content;
		if (is_in_range(val_t,
				negative_touch->near_touch,
				negative_touch->far_touch))
			return (true);
		lst_neg = lst_neg->next;
	}
	return (false);
}

void			get_negative_touch(t_list **lst, const t_env *env, t_ray *ray)
{
	t_list		*lst_neg;
	uint64_t	i;
	t_dvec3		touch;
	t_touch		touch_tmp;

	lst_neg = NULL;
	i = UINT64_MAX;
	while (++i < env->neg_arr_len)
	{
		env->neg_arr[i].get_intersect(&touch, &env->neg_arr[i], ray);
		touch_tmp = (t_touch){&env->neg_arr[i], touch.x, touch.y};
		ft_lstappend(&lst_neg, &touch_tmp, sizeof(t_touch));
	}
	*lst = lst_neg;
}
