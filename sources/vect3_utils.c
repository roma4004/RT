/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 15:28:04 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/24 10:10:34 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void		vec3_to_negative(t_dvec3 *restrict destination)
{
	destination->x = -destination->x;
	destination->y = -destination->y;
	destination->z = -destination->z;
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

//
//float		vec3_mag(const t_fvec3 *restrict first)
//{
//	return (sqrtf(powf(first->x, 2) + powf(first->y, 2) + powf(first->z, 2)));
//}
//
//void		vec3_normalize(t_fvec3 *restrict destination,
//							t_fvec3 *restrict first)
//{
//	float magnitude = vec3_mag(first);
//
//	if (magnitude > 0)
//	{
//		vec3_div_float(destination, first, magnitude);
//	}
//
//}
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
	{
//		t_fvec3 tmp = (t_fvec3){1,1,1};
//		vec3_div_double2(destination, &tmp, magnitude);
		vec3_div_double2(destination, first, magnitude);
//		vec3_mul_vec3(destination, destination, first);
	}

}

double			vec3_length(t_dvec3 vec)
{
	return (sqrt(vec3_dot_vec3(vec, vec)));
}