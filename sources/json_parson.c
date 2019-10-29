/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parson.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykopiika <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 16:49:28 by ykopiika          #+#    #+#             */
/*   Updated: 2019/10/29 16:21:19 by ykopiika         ###   ########.fr       */
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
	t_calc_arr	var;

	var.type = 424242;
	var.i = 0;
	if (!count_n_malloc(&var, obj_arr, arr_len, arr))
		return (false);
	var.i_js = 0;
	var.i = 0;
	while (var.i < var.js_len)
	{
		var.object = json_array_get_object(arr, var.i);
		if (get_type_obj(var.object, &var.type) == false)
			return (false);
		if (parse_obj(var.object, *obj_arr, var.type, &var.i_js) == false)
			return (false);
		var.i++;
		var.i_js++;
	}
	return (true);
}

static _Bool	parse_ligt_arr(JSON_Array *arr, t_lght **lght_arr,
								size_t *arr_len)
{
	JSON_Object	*object;
	size_t		i;
	size_t		type;

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
	_Bool		flag;

	flag = true;
	if ((array = json_object_get_array(object, "objects")) == NULL)
		return (false);
	if ((parse_uni_or_neg(array, &env->uni_arr, &env->uni_arr_len) == false))
		return (false);
	if ((array = json_object_get_array(object, "negative_objects")) == NULL)
		flag = false;
	if (flag && !(parse_uni_or_neg(array, &env->neg_arr, &env->neg_arr_len)))
		return (false);
	if ((array = json_object_get_array(object, "lights")) == NULL)
		return (false);
	if ((parse_ligt_arr(array, &env->light_arr, &env->light_arr_len) == false))
		return (false);
	if ((array = json_object_get_array(object, "parameters")) == NULL)
		return (false);
	if ((parse_params(env, array) == false))
		return (false);
	return (true);
}

_Bool			json_parson(t_env *env, char *file_name, Uint32 *err_id)
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
