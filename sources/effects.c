/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 20:50:03 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/16 20:50:03 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void				apply_sepia_filter(t_dvec3 *color)
{
	*color = (t_dvec3){
		.x = (color->x * 0.393) + (color->y * 0.769) + (color->z * 0.189),
		.y = (color->x * 0.349) + (color->y * 0.686) + (color->z * 0.168),
		.z = (color->x * 0.272) + (color->y * 0.534) + (color->z * 0.131)};
	ft_clamp_in_range_vec(color, 0, 255);
}