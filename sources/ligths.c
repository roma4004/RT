/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ligths.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 19:24:07 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/07 19:27:18 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

float				get_light(t_fvec3 point, t_fvec3 normal,
							   t_lght *lights_arr)
{
	float intensity = 0;
	float length_n = Length(normal);  // Should be 1.0, but just in case...
	for (unsigned i = 0; i < LIGHTS_CNT; i++)
	{
		t_lght light = lights_arr[i];
		if (light.type == LIGHT_AMBIENT) {
			intensity += light.intensity;
		}
		else
		{
			t_fvec3		vec_l;
			if (light.type == LIGHT_POINT)
			{
				vec_l = vec3_sub_vec3(light.position, point);
			}
			else
			{  // LIGHT_DIRECTIONAL
				vec_l = light.position;
			}
			float n_dot_l = vec3_dot_vec3(normal, vec_l);
			if (n_dot_l > 0)
			{
				intensity += light.intensity * n_dot_l / (length_n * Length(vec_l));
			}
		}
	}
	return (intensity);
}