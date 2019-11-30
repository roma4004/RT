/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_validate_compare.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dromanic <dromanic@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 20:03:08 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 20:27:36 by dromanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		cmp_begin(const char *s1, const char *s2)
{
	uint64_t	i;

	i = 0;
	while (s1[i]
	&& (s1[i] == s2[i]))
		i++;
	if (i == ft_strlen(s1))
		return (1);
	return (0);
}

uint64_t		get_type_neg(const char *str)
{//todo: add in cmp_begin case insensitive compare logic
	if (cmp_begin("sphereneg", str)
		|| cmp_begin("SPHERENEG", str))
		return (SPHERENEG);
	else if (cmp_begin("planeneg", str)
		|| cmp_begin("PLANENEG", str))
		return (PLANENEG);
	else if (cmp_begin("coneneg", str)
		|| cmp_begin("CONENEG", str))
		return (CONENEG);
	else if (cmp_begin("cylinderneg", str)
		|| cmp_begin("CYLINDERNEG", str))
		return (CYLINDERNEG);
	else if (cmp_begin("diskneg", str)
		|| cmp_begin("DISKNEG", str))
		return (DISKNEG);
	else if (cmp_begin("paraboloidneg", str)
		|| cmp_begin("PARABOLOIDNEG", str))
		return (PARABOLOIDNEG);
	return (UINT64_MAX);
}

uint64_t		get_type_uni(const char *str)
{//todo: add in cmp_begin case insensitive compare logic
	if (cmp_begin("sphere", str)
	|| cmp_begin("SPHERE", str))
		return (SPHERE);
	else if (cmp_begin("plane", str)
		|| cmp_begin("PLANE", str))
		return (PLANE);
	else if (cmp_begin("cone", str)
		|| cmp_begin("CONE", str))
		return (CONE);
	else if (cmp_begin("cylinder", str)
		|| cmp_begin("CYLINDER", str))
		return (CYLINDER);
	else if (cmp_begin("disk", str)
		|| cmp_begin("DISK", str))
		return (DISK);
	else if (cmp_begin("paraboloid", str)
		|| cmp_begin("PARABOLOID", str))
		return (PARABOLOID);
	return (UINT64_MAX);
}

uint64_t		get_type_other(const char *str)
{//todo: add in cmp_begin case insensitive compare logic
	if (cmp_begin("cam", str)
	|| cmp_begin("CAM", str))
		return (CAM);
	else if (cmp_begin("ambient", str)
		|| cmp_begin("AMBIENT", str))
		return (L_AMBIENT);
	else if (cmp_begin("point", str)
		|| cmp_begin("POINT", str))
		return (L_POINT);
	else if (cmp_begin("screen", str)
		|| cmp_begin("SCREEN", str))
		return (SCRN);
	else if (cmp_begin("directional", str)
		|| cmp_begin("DIRECTIONAL", str))
		return (L_DIRECTIONAL);
	return (UINT64_MAX);
}

uint64_t		get_type(const char *str)
{
	uint64_t	type;

	if (str[0] == '#'
	|| str[0] == ' ')
		return (UINT64_MAX);
	if ((type = get_type_neg(str)) != UINT64_MAX)
		return (type);
	if ((type = get_type_uni(str)) != UINT64_MAX)
		return (type);
	if ((type = get_type_other(str)) != UINT64_MAX)
		return (type);
	return (UINT64_MAX);
}
