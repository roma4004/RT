/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_with_vect3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:33:33 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/04 17:04:18 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	vec3_add_vec3(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = first->x + second->x,
							.y = first->y + second->y,
							.z = first->z + second->z};
}

void	vec3_sub_vec3(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = first->x - second->x,
							.y = first->y - second->y,
							.z = first->z - second->z};
}

void	vec3_mul_vec3(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = first->x * second->x,
							.y = first->y * second->y,
							.z = first->z * second->z};
}

void	vec3_div_vec3(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						const t_dvec3 *restrict second)
{
	if (second->x == 0.0
	|| second->y == 0.0
	|| second->z == 0.0)
		return ;
	*destination = (t_dvec3){.x = first->x / second->x,
							.y = first->y / second->y,
							.z = first->z / second->z};
}

void	vec3_crs_vec3(t_dvec3 *destination,
						const t_dvec3 *restrict first,
						const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = first->y * second->z - first->z * second->y,
							.y = first->z * second->x - first->x * second->z,
							.z = first->x * second->y - first->y * second->x};
}
