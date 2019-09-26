/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 15:28:04 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/26 14:40:22 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static uint8_t  double2byte_clamp(const double x)
{
	if (x < 0.0)
		return (0);
	else if (x > 1.0)
		return (255);
	else
		return ((uint8_t)(255.0 * x));
}

t_dvec3				double_mul_vec3_col(double first, t_dvec3 second)
{
	return ((t_dvec3){double2byte_clamp(first * second.x),
					  double2byte_clamp(first * second.y),
					  double2byte_clamp(first * second.z)});
}

t_dvec3				vec3_add_vec3_col(t_dvec3 first, t_dvec3 second)
{
	return ((t_dvec3){double2byte_clamp(first.x + second.x),
					  double2byte_clamp(first.y + second.y),
					  double2byte_clamp(first.z + second.z)});
}

double		vec3_to_double(t_dvec3 first)
{
	return (first.x + first.y + first.z);
}

double		vec3_dot_vec3(t_dvec3 first, t_dvec3 second)
{
	return (first.x * second.x
		+ first.y * second.y
		+ first.z * second.z);
}

double		vec3_mag(const t_dvec3 *restrict first)
{
	return (sqrtf(powf(first->x, 2.0f)
				+ powf(first->y, 2.0f)
				+ powf(first->z, 2.0f)));
}

void		vec3_normalize(t_dvec3 *restrict destination,
							t_dvec3 *restrict first)
{// Should be 1.0, but just in case...
	double magnitude = vec3_mag(first);

	if (magnitude > 0)
		vec3_div_double2(destination, first, magnitude);
}

double			vec3_length(t_dvec3 vec)
{
	return (sqrt(vec3_dot_vec3(vec, vec)));
}