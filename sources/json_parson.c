/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_parson.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykopiika <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 16:49:28 by ykopiika          #+#    #+#             */
/*   Updated: 2019/10/27 14:18:47 by ykopiika         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//_Bool	pr_object(JSON_Object *obj, t_fgrs *fgrs)
//{
//	char	*obj_type;
//
//	obj_type = (char*)json_object_get_string(obj, "type");
//	if (obj_type == NULL)
//		print_error(ERR_ARGV, -1);
//	else if (ft_strcmp(obj_type, "sphere") == 0)
//		get_sphere_val(fgrs, obj);
//	else if (ft_strcmp(obj_type, "plane") == 0)
//		get_plane_val(fgrs, obj);
//	if (ft_strcmp(obj_type, "cylinder") == 0)
//		get_cylinder_val(fgrs, obj);
//	else if (ft_strcmp(obj_type, "cone") == 0)
//		get_cone_val(fgrs, obj);
//	else
//		return (0);
//	return (1);
//}
//
//_Bool	parse_array_of_scene_objects(JSON_Array *j_arr, t_data *d)
//{
//	JSON_Object	*j_ob;
//	t_fgrs	*fgrs;
//	int i;
//	int len;
//
//	i = 0;
//	len = json_array_get_count(j_arr);
//	fgrs = (t_fgrs *)malloc(sizeof(t_fgrs));
//	d->figures = fgrs;
//	while (i < len)
//	{
//		if (i > 0)
//		{
//			fgrs->next = (t_fgrs *)malloc(sizeof(t_fgrs));
//			ft_bzero(fgrs->next, sizeof(t_fgrs));
//			fgrs = fgrs->next;
//		}
//		j_ob = json_array_get_object(j_arr, i++);
//
//		pr_object(j_ob, fgrs);
////		if (!)
////		{
////			ft_putstr("Error while parsing objects");
////			exit(ERR_ARGV);
////		}
//	}
//	fgrs->next = NULL;
//	return (1);
//}
//
//void	rt_parse_objs_n_light(t_data *d, JSON_Object *json_objs)
//{
//	JSON_Array	*json_arr;
//
//	if ((json_arr = json_object_get_array(json_objs, "objects")) == NULL)
//	{
//		ft_putstr("Error while getting array of scene objects");
//		exit(ERR_ARGV);
//	}
//
//	parse_array_of_scene_objects(json_arr, d);
//
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
//}
//
//void json_parson(t_data *d, char *file_name)
//{
//	JSON_Value *value;
//	JSON_Object *object;
//
//	value = json_parse_file(file_name);
//	if (value == NULL)
//	{
//		ft_putstr("Error: File is invalid\n");
//		exit(ERR_ARGV);
//	}
//	object = json_value_get_object(value);
//	if (object == NULL)
//	{
//		ft_putstr("Error: Objects in file is invalid\n");
//		exit(ERR_ARGV);
//	}
//
//
//	rt_parse_objs_n_light(d, object);
////	--------------------------------------------------------------
//	if (!(pr_camera(object, &(rt->camera))))
//	{
//		ft_putstr("Error: CAMERA in file is invalid\n");
//		exit(ERR_ARGV);
//	}
//
//
//	json_object_clear(object);
//	json_value_free(value);
//
//}

