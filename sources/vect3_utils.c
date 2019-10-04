/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 15:28:04 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/04 12:35:34 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

double		vec3_dot_vec3(const t_dvec3 *restrict first, const t_dvec3 *restrict second)
{
	return (first->x * second->x
			+ first->y * second->y
			+ first->z * second->z);
}

double		vec3_length(const t_dvec3 *restrict first)
{
	return (sqrt(first->x * first->x
				+ first->y * first->y
				+ first->z * first->z));
}

void		vec3_normalize(t_dvec3 *destination, const t_dvec3 *restrict first)
{
	const double		magnitude = vec3_length(first);

	if (magnitude > 0.0)
		*destination = vec3_div_double(first, magnitude);
}
