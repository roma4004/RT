/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parson.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykopiika <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 16:49:28 by ykopiika          #+#    #+#             */
/*   Updated: 2019/10/28 21:41:51 by ykopiika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static _Bool	json_clear(JSON_Value **value, JSON_Object **object)
{
	json_object_clear(*object);
	json_value_free(*value);
	return (true);
}

static _Bool	parse_uni_or_neg(JSON_Array *arr, t_uni **obj_arr,
								 size_t *arr_len)
{
	JSON_Object	*object;
	size_t i;
	size_t len;
	size_t type;

	type = 424242;
	i = 0;
	*arr_len = json_array_get_count(arr);
	len = *arr_len;

	while (i < len)
	{
		object = json_array_get_object(arr, i);
		if (get_type_obj(object, &type) == false)
			return (false);
		if (type == CONE)
			len++;
		if (type == CYLINDER)
			len += 2;
		i++;
	}

//	caps = 0;
//	uni_arr[i_js + caps]

	*obj_arr = (t_uni *)malloc(sizeof(t_uni) * *arr_len);
	if (*obj_arr == NULL)
		return (false);
	while (i < *arr_len)
	{
		ft_bzero((*obj_arr) + i, sizeof(t_uni));
		object = json_array_get_object(arr, i);
		if (get_type_obj(object, &type) == false)
			return (false);
		if (parse_obj(object, (*obj_arr), type, &i) == false)
			return (false);
		i++;
	}
	return (true);
}

static _Bool	parse_ligt_arr(JSON_Array *arr, t_lght **lght_arr,
								 size_t *arr_len)
{
	JSON_Object	*object;
	size_t i;
	size_t type;

	type = 424242;
	i = 0;
	*arr_len = json_array_get_count(arr);
	*lght_arr = (t_lght *)malloc(sizeof(t_lght) * *arr_len);
	if (*lght_arr == NULL)
		return (false);
	while (i < *arr_len)
	{
		ft_bzero((*lght_arr) + i, sizeof(t_lght));
		object = json_array_get_object(arr, i);
		if (get_type_light(object, &type) == false)
			return (false);
		if (parse_light(object, (*lght_arr) + i, type) == false)
			return (false);
		i++;
	}
	return (true);
}

static _Bool	parse_all_objects(t_env *env, JSON_Object *object)
{
	JSON_Array	*array;

	if ((array = json_object_get_array(object, "objects")) == NULL)
		return (false);
	if ((parse_uni_or_neg(array, &env->uni_arr, &env->uni_arr_len) == false))
		return (false);
	if ((array = json_object_get_array(object, "negative_objects")) == NULL)
		return (false);
	if ((parse_uni_or_neg(array, &env->neg_arr, &env->neg_arr_len) == false))
		return (false);
	if ((array = json_object_get_array(object, "lights")) == NULL)
		return (false);
	if ((parse_ligt_arr(array, &env->light_arr, &env->light_arr_len) == false))
		return (false);
	if ((array = json_object_get_array(object, "parameters")) == NULL)
		return (false);
	if ((parse_params(array, env) == false))
		return (false);
	return (true);
}

_Bool json_parson(t_env *env, char *file_name, Uint32 *err_id)
{
	JSON_Value	*value;
	JSON_Object	*object;

	value = json_parse_file(file_name);
	if (value == NULL && (*err_id = ERR_READ))
		return (false);
	object = json_value_get_object(value);
	if (object == NULL && (*err_id = ERR_SCENE))
		return (false);
	if ((parse_all_objects(env, object)) == false
		&& (*err_id = ERR_SCENE)
		&& (json_clear(&value, &object)))
		return (false);
	return (true);
}

