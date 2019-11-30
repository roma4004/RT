/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_get_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykopiika <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 19:00:46 by ykopiika          #+#    #+#             */
/*   Updated: 2019/10/29 20:41:11 by ykopiika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool	get_type_obj(JSON_Object *obj, uint64_t *type)
{
	char	*obj_type;

	obj_type = (char*)json_object_get_string(obj, "type");
	if (obj_type == NULL)
		return (false);
	else if (ft_strcmp(obj_type, "sphere") == 0)
		*type = SPHERE;
	else if (ft_strcmp(obj_type, "plane") == 0)
		*type = PLANE;
	else if (ft_strcmp(obj_type, "cylinder") == 0)
		*type = CYLINDER;
	else if (ft_strcmp(obj_type, "cone") == 0)
		*type = CONE;
	else if (ft_strcmp(obj_type, "paraboloid") == 0)
		*type = PARABOLOID;
	else
		return (false);
	return (true);
}

_Bool	get_type_light(JSON_Object *obj, uint64_t *type)
{
	char	*obj_type;

	obj_type = (char*)json_object_get_string(obj, "type");
	if (obj_type == NULL)
		return (false);
	else if (ft_strcmp(obj_type, "ambient") == 0)
		*type = L_AMBIENT;
	else if (ft_strcmp(obj_type, "point") == 0)
		*type = L_POINT;
	else if (ft_strcmp(obj_type, "directional") == 0)
		*type = L_DIRECTIONAL;
	else
		return (false);
	return (true);
}

_Bool	get_uint_val(Uint32 *dst, char *key_word, JSON_Object *obj)
{
	double res;

	if (json_object_has_value_of_type(obj, key_word, JSONNumber) == 0)
		return (0);
	res = json_object_get_number(obj, key_word);
	*dst = (Uint32)res;
	return (1);
}

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
	if (array == NULL || !dst)
		return (0);
	if (json_array_get_count(array) != 3)
	{
		*dst = (t_dvec3) {0, 0, 0, 0};
		return (0);
	}
	i = -1;
	while (++i < 3)
	{
		value = json_array_get_value(array, i);
		if (json_value_get_type(value) != JSONNumber)
			return (0);
		res[i] = json_value_get_number(value);
	}
	*dst = (t_dvec3){res[0], res[1], res[2], 0};
	return (1);
}
