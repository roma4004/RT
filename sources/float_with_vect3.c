/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_with_vect3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:39:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/08/13 13:40:59 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				float_add_vec3(t_fvec3 *restrict destination,
									const float first,
									const t_fvec3 *restrict second)
{
	destination->x = first + second->x;
	destination->y = first + second->y;
	destination->z = first + second->z;
}

void				float_sub_vec3(t_fvec3 *restrict destination,
									const float first,
									const t_fvec3 *restrict second)
{
	destination->x = first - second->x;
	destination->y = first - second->y;
	destination->z = first - second->z;
}

void				float_mul_vec3(t_fvec3 *restrict destination,
									const float first,
									const t_fvec3 *restrict second)
{
	destination->x = first * second->x;
	destination->y = first * second->y;
	destination->z = first * second->z;
}

void				float_div_vec3(t_fvec3 *restrict destination,
									const float first,
									const t_fvec3 *restrict second)
{
	destination->x = first / second->x;
	destination->y = first / second->y;
	destination->z = first / second->z;
}


