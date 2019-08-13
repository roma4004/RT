/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_with_float.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:34:32 by dromanic          #+#    #+#             */
/*   Updated: 2019/08/13 13:37:40 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				vec3_add_float(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const float second)
{
	destination->x = first->x + second;
	destination->y = first->y + second;
	destination->z = first->z + second;
}

void				vec3_sub_float(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const float second)
{
	destination->x = first->x - second;
	destination->y = first->y - second;
	destination->z = first->z - second;
}

void				vec3_mul_float(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const float second)
{
	destination->x = first->x * second;
	destination->y = first->y * second;
	destination->z = first->z * second;
}

void				vec3_div_float(t_fvec3 *restrict destination,
							const t_fvec3 *restrict first,
							const float second)
{
	destination->x = first->x / second;
	destination->y = first->y / second;
	destination->z = first->z / second;
}
