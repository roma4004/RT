/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_get_obj.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 17:01:42 by ykopiika          #+#    #+#             */
/*   Updated: 2019/10/30 04:01:09 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Bool		get_param(JSON_Object *jsn_obj, t_uni *obj)
{
	if (!get_vector_val(&obj->pos, POSITION, jsn_obj)
	|| !get_double_val(&obj->radius, RADIUS, jsn_obj)
	|| !get_vector_val(&obj->dir, DIRECTION, jsn_obj)
	|| !get_double_val(&obj->height, HEIGHT, jsn_obj)
	|| !get_vector_val(&obj->color, COLOR, jsn_obj)
	|| !get_double_val(&obj->specular, SPCULAR, jsn_obj)
	|| !get_double_val(&obj->reflective_coef, RFLCTV, jsn_obj)
	|| !get_double_val(&obj->refractive_coef, RFRCTV, jsn_obj)
	|| !get_double_val(&obj->texture_id, TEXTURE_ID, jsn_obj))
		return (false);
	return (true);
}

_Bool		parse_obj(JSON_Object *jsn_obj, t_uni *obj,
				uint64_t type, uint64_t *i)
{
	get_param(jsn_obj, &obj[*i]);
	obj[*i].height = fabs(obj[*i].height);
	obj[*i].color = vec3_clamp_col_cpy(obj[*i].color);
	vec3_normalize(&obj[*i].dir, &obj[*i].dir);
	set_backup_val(&obj[*i]);
	if (obj[*i].dir.x == 0.0 && obj[*i].dir.y == 0.0 && obj[*i].dir.z == 0.0)
		obj[*i].dir = (t_dvec3){0.0, 1.0, 0.0, 0.0};
	ft_clamp_in_range(&obj[*i].reflective_coef,
			obj[*i].reflective_coef, 0.0, 1.0);
	ft_clamp_in_range(&obj[*i].refractive_coef,
			obj[*i].refractive_coef, 0.0, 1.0);
	obj[*i].cone_angle_cache = (obj[*i].height == 0.0) ? 0.0
							: obj[*i].radius / obj[*i].height;
	obj[*i].get_intersect = g_intersect_catalog(type);
	obj[*i].get_normal = g_normal_catalog(type);
	add_caps(obj, i, type);
	return (true);
}

_Bool		parse_light(JSON_Object *jsn_obj, t_lght *lght, uint64_t type)
{
	lght->type = type;
	if (!get_vector_val(&lght->col, COLOR, jsn_obj)
	|| !get_double_val(&lght->intensity, INTENSITY, jsn_obj)
	|| !get_vector_val(&lght->pos, POSITION, jsn_obj))
		return (false);
	lght->col = vec3_clamp_col_cpy(lght->col);
	vec3_div_double(&lght->col, &lght->col, 255.0);
	ft_clamp_in_range(&lght->intensity, lght->intensity, 0.0, 1.0);
	vec3_mul_double(&lght->col, &lght->col, lght->intensity);
	return (true);
}

_Bool		parse_params(t_env *env, JSON_Array *arr)
{
	JSON_Object		*jsn_obj;
	double			res;

	if (json_array_get_count(arr) != 1)
		return (false);
	jsn_obj = json_array_get_object(arr, 0);
	ft_bzero(&env->cam, sizeof(t_cam));
	if (!get_vector_val(&env->cam.pos, POSITION, jsn_obj)
	|| !get_vector_val(&env->cam.rotate_angle, "rotate angle", jsn_obj)
	|| !get_uint_val(&env->buff_width, "width", jsn_obj)
	|| !get_uint_val(&env->buff_height, "height", jsn_obj))
		return (false);
	res = env->buff_width;
	ft_clamp_in_range(&res, res, 200.0, 2000.0);
	env->buff_width = (Uint32)res;
	res = env->buff_height;
	ft_clamp_in_range(&res, res, 200.0, 2000.0);
	env->buff_height = (Uint32)res;
	env->cam.pos_backup = env->cam.pos;
	env->cam.rotate_angle_backup = env->cam.rotate_angle;
	return (true);
}

_Bool		count_n_malloc(t_calc_arr *var, t_uni **obj_arr,
				uint64_t *arr_len, JSON_Array *arr)
{
	var->js_len = json_array_get_count(arr);
	*arr_len = var->js_len;
	while (var->i < var->js_len)
	{
		var->object = json_array_get_object(arr, var->i);
		if (get_type_obj(var->object, &var->type) == false)
			return (false);
		if (var->type == CONE)
			*arr_len = *arr_len + 1;
		if (var->type == CYLINDER)
			*arr_len = *arr_len + 2;
		var->i++;
	}
	if (!(*obj_arr = (t_uni *)malloc(sizeof(t_uni) * *arr_len)))
		return (false);
	ft_bzero(*obj_arr, sizeof(t_uni) * *arr_len);
	return (true);
}
