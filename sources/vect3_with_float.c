/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_with_float.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:34:32 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/07 17:52:46 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				vec3_add_float2(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const float second)
{
	destination->x = first->x + second;
	destination->y = first->y + second;
	destination->z = first->z + second;
}

//translate
void				vec3_sub_float2(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const float second)
{
	destination->x = first->x - second;
	destination->y = first->y - second;
	destination->z = first->z - second;
}

//scale
void				vec3_mul_float2(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const float second)
{
	destination->x = first->x * second;
	destination->y = first->y * second;
	destination->z = first->z * second;
}

void				vec3_div_float2(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const float second)
{
	if (second == 0.f)
		return ;
	destination->x = first->x / second;
	destination->y = first->y / second;
	destination->z = first->z / second;
}

////////////////////

t_fvec3				vec3_add_float(t_fvec3 first, float second)
{
	return ((t_fvec3){first.x + second,
						first.y + second,
						first.z + second});
}

//translate
t_fvec3				vec3_sub_float(t_fvec3 first, float second)
{
	return ((t_fvec3){first.x - second,
						first.y - second,
						first.z - second});
}

//scale
t_fvec3				vec3_mul_float(t_fvec3 first, float second)
{
	return ((t_fvec3){first.x * second,
						first.y * second,
						first.z * second});
}

t_fvec3				vec3_div_float(t_fvec3 first, float second)
{
//	if (second == 0.f)
//		return (first);
	return ((t_fvec3){first.x / second,
						first.y / second,
						first.z / second});
}
