/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_with_vect3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 13:39:29 by dromanic          #+#    #+#             */
/*   Updated: 2019/09/29 11:43:00 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
	if (second.x == 0.0
	|| second.y == 0.0
	|| second.z == 0.0)
		return (second);
	return ((t_dvec3){first / second.x,
						first / second.y,
						first / second.z});
}
