/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_validate_scene.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtlostiu <vtlostiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 15:31:32 by dromanic          #+#    #+#             */
/*   Updated: 2019/10/29 16:53:28 by vtlostiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		is_dot_in_double(const char *str, size_t i)
{
	if (str
	&& i != 0
	&& ft_isdigit(str[i + 1])
	&& ft_isdigit(str[i - 1]))
		return (1);
	return (0);
}

_Bool			is_valid_line(t_env *env, char **line, size_t len)
{
	char		*str;
	size_t		i;

	if (!line || !(str = *line))
		return (false);
	i = UINT64_MAX;
	while (++i < len && str[i] != '#')
	{
		if (!((str[i] == '+' && ft_isdigit(str[i + 1]))
		|| (str[i] == '-' && ft_isdigit(str[i + 1]))
		|| (str[i] == '.' && is_dot_in_double(str, i))
		|| ft_isalpha(str[i])
		|| ft_isdigit(str[i])
		|| ft_strchr(" \t|", str[i])))
		{
			env->flags.err_id = ERR_SCENE;
			ft_strdel(line);
			return (false);
		}
	}
	return (true);
}

size_t			count_number(t_env *env, char *str, size_t len)
{
	size_t		i;
	size_t		digits;
	size_t		dots;

	i = UINT64_MAX;
	digits = 0;
	while (++i < len && str[i] != '#')
	{
		if (ft_isdigit(str[i]) && !(dots = 0) && ++digits)
		{
			while (i < len && str[i] != '#'
				&& (ft_isdigit(str[i]) || str[i] == '.'))
			{
				if (str[i] == '.')
					dots++;
				i++;
			}
			if (dots > 1)
				env->flags.err_id = ERR_SCENE;
		}
	}
	return (digits);
}
