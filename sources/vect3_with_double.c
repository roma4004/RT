/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_with_double.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:34:32 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/01 13:49:59 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

t_dvec3				vec3_add_double(const t_dvec3 *restrict first, double second)
{
	return ((t_dvec3){first->x + second,
						first->y + second,
						first->z + second,
						0.0});
}

t_dvec3				vec3_sub_double(const t_dvec3 *restrict first, double second)
{
	return ((t_dvec3){first->x - second,
						first->y - second,
						first->z - second,
						0.0});
}

t_dvec3				vec3_mul_double(const t_dvec3 *restrict first, double second)
{
	return ((t_dvec3){first->x * second,
						first->y * second,
						first->z * second,
						0.0});
}

t_dvec3				vec3_div_double(const t_dvec3 *restrict first, double second)
{
	if (second == 0.0)
		return (*first);
	return ((t_dvec3){first->x / second,
						first->y / second,
						first->z / second,
						0.0});
}
