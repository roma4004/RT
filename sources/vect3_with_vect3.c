/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_with_vect3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:33:33 by dromanic          #+#    #+#             */
/*   Updated: 2019/08/13 15:29:23 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				vec3_add_vec3(t_fvec3 *restrict destination,
									const t_fvec3 *restrict first,
									const t_fvec3 *restrict second)
{
	destination->x = first->x + second->x;
	destination->y = first->y + second->y;
	destination->z = first->z + second->z;
}

void				vec3_sub_vec3(t_fvec3 *restrict destination,
									const t_fvec3 *restrict first,
									const t_fvec3 *restrict second)
{
	destination->x = first->x - second->x;
	destination->y = first->y - second->y;
	destination->z = first->z - second->z;
}

void				vec3_mul_vec3(t_fvec3 *restrict destination,
									const t_fvec3 *restrict first,
									const t_fvec3 *restrict second)
{
	destination->x = first->x * second->x;
	destination->y = first->y * second->y;
	destination->z = first->z * second->z;
}

void				vec3_div_vec3(t_fvec3 *restrict destination,
									const t_fvec3 *restrict first,
									const t_fvec3 *restrict second)
{
	if (second == 0)
		return ;
	destination->x = first->x / second->x;
	destination->y = first->y / second->y;
	destination->z = first->z / second->z;
}

