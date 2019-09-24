/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_with_vect3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:33:33 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/07 16:51:45 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				vec3_add_vec32(t_dvec3 *restrict destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second)
{
	destination->x = first->x + second->x;
	destination->y = first->y + second->y;
	destination->z = first->z + second->z;
}

void				vec3_sub_vec32(t_dvec3 *restrict destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second)
{
	destination->x = first->x - second->x;
	destination->y = first->y - second->y;
	destination->z = first->z - second->z;
}

void				vec3_mul_vec32(t_dvec3 *restrict destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second)
{
	destination->x = first->x * second->x;
	destination->y = first->y * second->y;
	destination->z = first->z * second->z;
}

void				vec3_div_vec32(t_dvec3 *restrict destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second)
{
	if (second == 0)
		return ;
	destination->x = first->x / second->x;
	destination->y = first->y / second->y;
	destination->z = first->z / second->z;
}

void				vec3_cross_vec32(t_dvec3 *restrict destination,
									const t_dvec3 *restrict first,
									const t_dvec3 *restrict second)
{
	destination->x = first->y * second->z - first->z * second->y;
	destination->y = first->z * second->x - first->x * second->z;
	destination->z = first->x * second->y - first->y * second->x;
}

////////////////////////////////

t_dvec3				vec3_add_vec3(t_dvec3 first, t_dvec3 second)
{
	return ((t_dvec3){first.x + second.x,
					  first.y + second.y,
					  first.z + second.z});
}

t_dvec3				vec3_sub_vec3(t_dvec3 first, t_dvec3 second)
{
	return ((t_dvec3){first.x - second.x,
					  first.y - second.y,
					  first.z - second.z});
}

t_dvec3				vec3_mul_vec3(t_dvec3 first, t_dvec3 second)
{
	return ((t_dvec3){first.x * second.x,
					  first.y * second.y,
					  first.z * second.z});
}

t_dvec3				vec3_div_vec3(t_dvec3 first, t_dvec3 second)
{
	if (second.x == 0.0f || second.y == 0.0f || second.z == 0.0f)
		return (first);
	return ((t_dvec3){first.x / second.x,
					  first.y / second.y,
					  first.z / second.z});
}

t_dvec3				vec3_cross_vec3(t_dvec3 first, t_dvec3 second)
{
	return ((t_dvec3){first.y * second.z - first.z * second.y,
					  first.z * second.x - first.x * second.z,
					  first.x * second.y - first.y * second.x});
}

