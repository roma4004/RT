/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect3_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 15:28:04 by dromanic          #+#    #+#             */
/*   Updated: 2019/08/13 18:33:06 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

float		vec3_to_float(const t_fvec3 *restrict first)
{
	return (first->x + first->y + first->z);
}

float		vec3_magnitude(const t_fvec3 *restrict first)
{
	return (sqrtf(first->x * first->x
				+ first->y * first->y
				+ first->z * first->z));
}

void		vec3_normalize(t_fvec3 *restrict destination,
							t_fvec3 *restrict first)
{
	float magnitude = vec3_magnitude(first);

	if (magnitude > 0)
	{
		vec3_div_float(destination, first, magnitude);
	}

}
void		vec3_normalize_ptr(t_fvec3 *restrict first)
{
	float m = vec3_magnitude(first);

	if (m > 0)
	{
		vec3_div_float(first, first, m);
	}
}

void		vec3_normalize_cpy(t_fvec3 first)
{
	float m = vec3_magnitude(&first);

	if (m > 0)
	{
		vec3_div_float(&first, &first, m);
	}
}