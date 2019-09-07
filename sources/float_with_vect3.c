/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_with_vect3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:39:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/07 17:52:47 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				float_add_vec32(t_fvec3 *restrict destination,
									const float first,
									const t_fvec3 *restrict second)
{
	destination->x = first + second->x;
	destination->y = first + second->y;
	destination->z = first + second->z;
}

void				float_sub_vec32(t_fvec3 *restrict destination,
									const float first,
									const t_fvec3 *restrict second)
{
	destination->x = first - second->x;
	destination->y = first - second->y;
	destination->z = first - second->z;
}

void				float_mul_vec32(t_fvec3 *restrict destination,
									const float first,
									const t_fvec3 *restrict second)
{
	destination->x = first * second->x;
	destination->y = first * second->y;
	destination->z = first * second->z;
}

void				float_div_vec32(t_fvec3 *restrict destination,
									const float first,
									const t_fvec3 *restrict second)
{
	if (second == 0)
		return ;
	destination->x = first / second->x;
	destination->y = first / second->y;
	destination->z = first / second->z;
}

//////////////////////////////////

t_fvec3				float_add_vec3(float first, t_fvec3 second)
{
	return ((t_fvec3){first + second.x,
					  first + second.y,
					  first + second.z});
}

t_fvec3				float_sub_vec3(float first, t_fvec3 second)
{
	return ((t_fvec3){first - second.x,
					  first - second.y,
					  first - second.z});
}

t_fvec3				float_mul_vec3(float first, t_fvec3 second)
{
	return ((t_fvec3){first * second.x,
					  first * second.y,
					  first * second.z});
}

t_fvec3				float_div_vec3(float first, t_fvec3 second)
{
//	if (second.x == 0 || second.y == 0 || second.z == 0)
//		return (second);
	return ((t_fvec3){first / second.x,
					  first / second.y,
					  first / second.z});
}


