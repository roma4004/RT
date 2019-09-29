/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_with_double.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:34:32 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/07 17:52:46 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

t_dvec3				vec3_add_double(t_dvec3 first, double second)
{
	return ((t_dvec3){first.x + second,
						first.y + second,
						first.z + second});
}

t_dvec3				vec3_sub_double(t_dvec3 first, double second)
{
	return ((t_dvec3){first.x - second,
						first.y - second,
						first.z - second});
}

t_dvec3				vec3_mul_double(t_dvec3 first, double second)
{
	return ((t_dvec3){first.x * second,
						first.y * second,
						first.z * second});
}

t_dvec3				vec3_div_double(t_dvec3 first, double second)
{
	if (second == 0.0)
		return (first);
	return ((t_dvec3){first.x / second,
						first.y / second,
						first.z / second});
}
