/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/08 20:15:19 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

float				get_light(t_fvec3 point, t_fvec3 normal,
								t_fvec3 view, int specular, t_lght *lights_arr)
{
	float		intensity;
	t_fvec		length;
	t_fvec3		vec_l;
	float		n_dot_l;
	float		r_dot_v;

	intensity = 0;
	length = (t_fvec){vec3_magnitude(&normal), vec3_magnitude(&view)}; // Should be 1.0, but just in case...
	for (unsigned i = 0; i < LIGHTS_CNT; i++)
	{
		t_lght light = lights_arr[i];
		if (light.type == LIGHT_AMBIENT)
			intensity += light.intensity;
		else
		{
			(light.type == LIGHT_POINT)
				? vec_l = vec3_sub_vec3(light.pos, point) : (vec_l = light.pos); //LIGHT_DIRECTIONAL
			n_dot_l = vec3_dot_vec3(normal, vec_l);// Diffuse reflection.
			if (n_dot_l > 0)
				intensity += light.intensity * n_dot_l
							/ (length.x * vec3_magnitude(&vec_l));
			if (specular != -1)// Specular reflection.
			{
				t_fvec3 vec_r = vec3_sub_vec3(float_mul_vec3(2.0
								* vec3_dot_vec3(normal, vec_l), normal), vec_l);
				if ((r_dot_v = vec3_dot_vec3(vec_r, view)) > 0)
					intensity += light.intensity * pow(r_dot_v
							/ (vec3_magnitude(&vec_r) * length.y), specular);
			}
		}
	}
	return ((intensity > 1.0) ? 1.0 : intensity);
}