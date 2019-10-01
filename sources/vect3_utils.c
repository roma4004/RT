/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 15:28:04 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 11:43:00 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

double		vec3_to_double(t_dvec3 first)
{
	return (first.x + first.y + first.z);
}

double		vec3_dot_vec3(t_dvec3 first, t_dvec3 second)
{
	return (first.x * second.x
			+ first.y * second.y
			+ first.z * second.z);
}

double		vec3_length(t_dvec3 first)
{
	return (sqrt(first.x * first.x
				+ first.y * first.y
				+ first.z * first.z));
}

t_dvec3		vec3_normalize(t_dvec3 first)
{
	const double		magnitude = vec3_length(first);

	if (magnitude > 0.0)
		return (vec3_div_double(first, magnitude));
	return (first);
}
