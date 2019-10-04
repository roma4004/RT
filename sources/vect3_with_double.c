/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_with_double.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:34:32 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/04 17:12:04 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	vec3_add_double(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						double second)
{
	*destination = (t_dvec3){.x = first->x + second,
							.y = first->y + second,
							.z = first->z + second};
}

void	vec3_sub_double(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						double second)
{
	*destination = (t_dvec3){.x = first->x - second,
							.y = first->y - second,
							.z = first->z - second};
}

void	vec3_mul_double(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						double second)
{
	*destination = (t_dvec3){.x = first->x * second,
							.y = first->y * second,
							.z = first->z * second};
}

void	vec3_div_double(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						double second)
{
	if (second == 0.0)
		return ;
	*destination = (t_dvec3){.x = first->x / second,
							.y = first->y / second,
							.z = first->z / second};
}
