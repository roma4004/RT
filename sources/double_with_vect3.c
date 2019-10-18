/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_with_vect3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:39:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:33:20 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	double_add_vec3(t_dvec3 *destination,
						double first,
						const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = first + second->x,
							.y = first + second->y,
							.z = first + second->z};
}

void	double_sub_vec3(t_dvec3 *destination,
						double first,
						const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = first - second->x,
							.y = first - second->y,
							.z = first - second->z};
}

void	double_mul_vec3(t_dvec3 *destination,
						double first,
						const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){.x = first * second->x,
							.y = first * second->y,
							.z = first * second->z};
}

void		double_div_vec3(t_dvec3 *destination,
							double first,
							const t_dvec3 *restrict second)
{
	if (second->x == 0.0
	|| second->y == 0.0
	|| second->z == 0.0)
		return ;
	*destination = (t_dvec3){.x = first / second->x,
							.y = first / second->y,
							.z = first / second->z};
}
