/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parson.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykopiika <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 16:49:28 by ykopiika          #+#    #+#             */
/*   Updated: 2019/10/27 21:41:13 by ykopiika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static _Bool	get_type_obj(JSON_Object *obj, size_t *type)
{
	char	*obj_type;

	obj_type = (char*)json_object_get_string(obj, "type");
	if (obj_type == NULL)
		return (false);
	else if (ft_strcmp(obj_type, "sphere") == 0)
		*type = SPHERE;
	else if (ft_strcmp(obj_type, "plane") == 0)
		*type = PLANE;
	if (ft_strcmp(obj_type, "cylinder") == 0)
		*type = CYLINDER;
	else if (ft_strcmp(obj_type, "cone") == 0)
		*type = CONE;
	else
		return (false);
	return (true);
}

static _Bool	parse_uni_or_neg(JSON_Array *arr, t_uni **obj_arr,
								 size_t *arr_len)
{
	JSON_Object	*object;
	size_t i;
	size_t type;

	type = 424242;
	i = 0;
	*arr_len = json_array_get_count(arr);
	*obj_arr = (t_uni *)malloc(sizeof(t_uni) * *arr_len);
	if (*obj_arr == NULL)
		return (false);
	while (i < *arr_len)
	{
		ft_bzero((*obj_arr) + i, sizeof(t_uni));
		object = json_array_get_object(arr, i);
		if (get_type_obj(object, &type) == false)
			return (false);
		if (parse_obj(object, (*obj_arr) + i, type) == false)
			return (false);
		i++;
	}
	return (true);
}

_Bool json_parson(t_env *env, char *file_name, Uint32 *err_id)
{
	JSON_Value	*value;
	JSON_Object	*object;
	JSON_Array	*array;

	value = json_parse_file(file_name);
	if (value == NULL && (*err_id = ERR_READ))
		return (false);
	object = json_value_get_object(value);
	if (object == NULL && (*err_id = ERR_SCENE))
		return (false);

	if ((array = json_object_get_array(object, "objects")) == NULL
		&& (*err_id = ERR_SCENE))
		return (false);
	if ((parse_uni_or_neg(array, &env->uni_arr, &env->uni_arr_len) == false)
		&& (*err_id = ERR_SCENE))
		return (false);

	if ((array = json_object_get_array(object, "negative_objects")) == NULL
		&& (*err_id = ERR_SCENE))
		return (false);
	if ((parse_uni_or_neg(array, &env->neg_arr, &env->neg_arr_len) == false)
		&& (*err_id = ERR_SCENE))
		return (false);
//	--------------------------------------------------------------
//	if ((json_arr = json_object_get_array(json_objs, "lights")) == NULL)
//	{
//		ft_putstr("Error while getting array of lights");
//		exit(ERR_ARGV);
//	}
//	if (!parse_array_of_lights(json_arr, d))
//	{
//		ft_putstr("Error while parsing lights");
//		exit(ERR_ARGV);
//	}
	if (!(pr_camera(object, &(rt->camera))))
	{
		ft_putstr("Error: CAMERA in file is invalid\n");
		exit(ERR_ARGV);
	}


	json_object_clear(object);
	json_value_free(value);

}

