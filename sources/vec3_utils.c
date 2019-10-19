/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 15:28:04 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/18 19:33:20 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	vec3_dot_vec3(double *destination,
			const t_dvec3 *restrict first,
			const t_dvec3 *restrict second)
{
	*destination = first->x * second->x
					+ first->y * second->y
					+ first->z * second->z;
}

void	vec3_length(double *destination,
			const t_dvec3 *restrict first)
{
	*destination = sqrt(first->x * first->x
				+ first->y * first->y
				+ first->z * first->z);
}

void	vec3_normalize(t_dvec3 *destination,
			const t_dvec3 *restrict first)
{
	double		magnitude;

	vec3_length(&magnitude, first);
	if (magnitude > 0.0)
		vec3_div_double(destination, first, magnitude);
}
