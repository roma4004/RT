/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_backup.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 19:55:43 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/20 15:05:05 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		set_backup_val(t_uni *obj)
{
	if (!obj)
		return ;
	obj->pos_backup = obj->pos;
	obj->radius_backup = obj->radius;
	obj->dir_backup = obj->dir;
}
