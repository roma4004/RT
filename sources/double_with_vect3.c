/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_with_vect3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:39:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/24 10:05:50 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				double_add_vec32(t_dvec3 *restrict destination,
									const double first,
									const t_dvec3 *restrict second)
{
	destination->x = first + second->x;
	destination->y = first + second->y;
	destination->z = first + second->z;
}

void				double_sub_vec32(t_dvec3 *restrict destination,
									const double first,
									const t_dvec3 *restrict second)
{
	destination->x = first - second->x;
	destination->y = first - second->y;
	destination->z = first - second->z;
}

void				double_mul_vec32(t_dvec3 *restrict destination,
									const double first,
									const t_dvec3 *restrict second)
{
	destination->x = first * second->x;
	destination->y = first * second->y;
	destination->z = first * second->z;
}

void				double_div_vec32(t_dvec3 *restrict destination,
									const double first,
									const t_dvec3 *restrict second)
{
	if (second == 0)
		return ;
	destination->x = first / second->x;
	destination->y = first / second->y;
	destination->z = first / second->z;
}

//////////////////////////////////

t_dvec3				double_add_vec3(double first, t_dvec3 second)
{
	return ((t_dvec3){first + second.x,
					  first + second.y,
					  first + second.z});
}

t_dvec3				double_sub_vec3(double first, t_dvec3 second)
{
	return ((t_dvec3){first - second.x,
					  first - second.y,
					  first - second.z});
}

t_dvec3				double_mul_vec3(double first, t_dvec3 second)
{
	return ((t_dvec3){first * second.x,
					  first * second.y,
					  first * second.z});
}

t_dvec3				double_div_vec3(double first, t_dvec3 second)
{
//	if (second.x == 0 || second.y == 0 || second.z == 0)
//		return (second);
	return ((t_dvec3){first / second.x,
					  first / second.y,
					  first / second.z});
}


