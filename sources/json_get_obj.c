/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_get_obj.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykopiika <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 17:01:42 by ykopiika          #+#    #+#             */
/*   Updated: 2019/10/28 21:32:06 by ykopiika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool	parse_obj(JSON_Object *jsn_obj, t_uni *obj, size_t type, size_t *i)
{
	if (!get_vector_val(&obj[*i].pos, POSITION, jsn_obj)
		|| !get_double_val(&obj[*i].radius, RADIUS, jsn_obj)
		|| !get_vector_val(&obj[*i].dir, DIRECTION, jsn_obj)
		|| !get_double_val(&obj[*i].height, HEIGHT, jsn_obj)
		|| !get_vector_val(&obj[*i].color, COLOR, jsn_obj)
		|| !get_double_val(&obj[*i].specular, SPCULAR, jsn_obj)
		|| !get_double_val(&obj[*i].reflective_coef, RFLCTV, jsn_obj)
		|| !get_double_val(&obj[*i].refractive_coef, RFRCTV, jsn_obj))
		return (false);
	obj[*i].height = fabs(obj[*i].height);
	obj[*i].color = vec3_clamp_col_cpy(obj[*i].color);
	vec3_normalize(&obj[*i].dir, &obj[*i].dir);
	set_backup_val(obj);
	if ((obj[*i].dir.x == 0.0) && (obj[*i].dir.y == 0.0)
		&& (obj[*i].dir.z == 0.0))
		obj[*i].dir = (t_dvec3){0.0, 1.0, 0.0, 0.0};
	ft_clamp_in_range(&obj[*i].reflective_coef,
			obj[*i].reflective_coef, 0.0, 1.0);
	ft_clamp_in_range(&obj[*i].refractive_coef,
			obj[*i].refractive_coef, 0.0, 1.0);
	obj[*i].cone_angle_cache = obj[*i].radius / obj[*i].height;
	obj[*i].get_intersect = g_intersect_catalog(type);
	obj[*i].get_normal = g_normal_catalog(type);
	add_caps(obj, i, type);
	return (true);
}

_Bool	parse_light(JSON_Object *jsn_obj, t_lght *lght, size_t type)
{
	lght->type = type;
	if (!get_vector_val(&lght->pos, POSITION, jsn_obj)
		|| !get_vector_val(&lght->color, COLOR, jsn_obj)
		|| !get_double_val(&lght->intensity, INTENSITY, jsn_obj))
		return (false);
	//TODO: clamp_parametrs
	return (true);
}

_Bool	parse_params(t_env *env, JSON_Array	*arr)
{
	JSON_Object	*jsn_obj;

	if(json_array_get_count(arr) != 1)
		return (false);
	jsn_obj = json_array_get_object(arr, 0);
	if (!get_vector_val(&env->cam.pos, POSITION, jsn_obj)
		|| !get_vector_val(&env->cam.rotate_angle, "rotate angle", jsn_obj)
		|| !get_uint_val(&env->buff_width, "width", jsn_obj)
		|| !get_uint_val(&env->buff_height, "height", jsn_obj))
		return (false);
	//TODO: clamp_parametrs
	env->cam.pos_backup = env->cam.pos;
	return (true);
}
