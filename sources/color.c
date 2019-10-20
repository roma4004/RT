/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 14:02:42 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/19 14:14:33 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_dvec3		vec3_clamp_col_cpy(t_dvec3 first)
{
	return ((t_dvec3){
		.x = double_clamp(first.x),
		.y = double_clamp(first.y),
		.z = double_clamp(first.z), 0.0});
}

void		double_mul_vec3_col(t_dvec3 *destination,
				double first,
				const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){
		.x = double_clamp(first * second->x),
		.y = double_clamp(first * second->y),
		.z = double_clamp(first * second->z)};
}

void		vec3_add_vec3_col(t_dvec3 *destination,
				const t_dvec3 *restrict first,
				const t_dvec3 *restrict second)
{
	*destination = (t_dvec3){
		.x = double_clamp(first->x + second->x),
		.y = double_clamp(first->y + second->y),
		.z = double_clamp(first->z + second->z)};
}

void		ft_clamp_in_range(double *dest, double value,
				double min, double max)
{
	if (value > max)
		*dest = max;
	else if (value < min)
		*dest = min;
	else
		*dest = value;
}

void		ft_clamp_in_range_vec(t_dvec3 *dest, double min, double max)
{
	ft_clamp_in_range(&dest->x, dest->x, min, max);
	ft_clamp_in_range(&dest->y, dest->y, min, max);
	ft_clamp_in_range(&dest->z, dest->z, min, max);
}
