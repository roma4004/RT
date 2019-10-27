/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_get_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykopiika <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 19:00:46 by ykopiika          #+#    #+#             */
/*   Updated: 2019/10/27 21:41:13 by ykopiika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool	get_double_val(double *dst, char *key_word, JSON_Object *obj)
{
	if (json_object_has_value_of_type(obj, key_word, JSONNumber) == 0)
		return (0);
	*dst = json_object_get_number(obj, key_word);
	return (1);
}

_Bool	get_vector_val(t_dvec3 *dst, char *key_word, JSON_Object *obj)
{
	int			i;
	double		res[3];
	JSON_Array	*array;
	JSON_Value	*value;

	array = json_object_get_array(obj, key_word);
	if (array == NULL)
		return (0);
	if (json_array_get_count(array) != 3)
		return (0);
	i = -1;
	while (++i < 3)
	{
		value = json_array_get_value(array, i);
		if (json_value_get_type(value) != JSONNumber)
			return (0);
		res[i] = json_value_get_number(value);
	}
	*dst = (t_dvec3){res[0],res[1],res[2],0};
	return (1);
}

_Bool	parse_obj(JSON_Object *jsn_obj, t_uni *obj, size_t type)
{
	if (!get_vector_val(&obj->pos, FGRS_CENTER, jsn_obj)
		|| !get_double_val(&obj->radius, FGRS_RADIUS, jsn_obj)
		|| !get_vector_val(&obj->dir, FGRS_DIR, jsn_obj)
		|| !get_double_val(&obj->height, FGRS_HEIGHT, jsn_obj)
		|| !get_vector_val(&obj->color, FGRS_COLOR, jsn_obj)
		|| !get_double_val(&obj->specular, FGRS_SPCUL, jsn_obj)
		|| !get_double_val(&obj->reflective_coef, FGRS_RFLCTV, jsn_obj)
		|| !get_double_val(&obj->refractive_coef, FGRS_RFRCTV, jsn_obj))
		return (false);
	//TODO: clamp_parametrs
	obj->pos_backup = obj->pos;
	obj->dir_backup = obj->dir;
	obj->radius_backup = obj->radius;
	obj->get_intersect = g_intersect_catalog(type);
	obj->get_normal = g_normal_catalog(type);
	//TODO: clculate cone_angle_cache
}
